#!/usr/bin/env python3
"""End-to-end checks. Every command launches a new process and reloads the file."""
import pathlib
import random
import subprocess
import sys
import tempfile

binary = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else './dbms').resolve()
checks = 0
with tempfile.TemporaryDirectory() as folder:
    state = pathlib.Path(folder) / 'state.data'
    def run(query, expected=None, error=False):
        global checks
        before = state.read_bytes() if state.exists() else None
        p = subprocess.run([str(binary), '--file', str(state), '--query', query], text=True, capture_output=True, timeout=10)
        assert (p.returncode != 0) == error, (query, p.returncode, p.stdout, p.stderr)
        if error:
            assert (state.read_bytes() if state.exists() else None) == before, query
        if expected is not None:
            assert p.stdout.split() == expected.split(), (query, expected, p.stdout)
        checks += 1
        return p.stdout

    run('MREAD absent', error=True)
    run('MCREATE a')
    run('MPUSH a one two three')
    run('MPUSHIDX a 1 middle')
    run('MSETIDX a 0 zero')
    run('MDELIDX a 2')
    run('MREAD a', 'zero middle three')
    run('MGET a 1', 'middle')
    run('MLENGTH a', '3')
    for query in ['MPUSHIDX a -1 bad', 'MGET a 3', 'MGET a 1x', 'MSETIDX a 9 bad',
                  'MDELIDX a 4', 'MPUSHIDX a 0', 'MPUSH a', 'MCREATE a', 'QPUSH a bad',
                  'BOGUS a', 'TDELETE t 1', 'MPUSH']:
        run(query, error=True)
    run('MPUSHIDX a 3 last')
    run('MREAD a', 'zero middle three last')

    for prefix in ['F', 'L']:
        name = prefix.lower()
        run(f'{prefix}CREATE {name}')
        # Empty-list boundaries must not dereference a null pointer.
        for query in [f'{prefix}POP {name}', f'{prefix}POPHEAD {name}',
                      f'{prefix}PUSHBEFORE {name} absent z', f'{prefix}PUSHAFTER {name} absent z',
                      f'{prefix}POPBEFORE {name} absent', f'{prefix}POPAFTER {name} absent']:
            run(query)
        run(f'{prefix}PUSH {name} b d')
        run(f'{prefix}PUSHHEAD {name} a')
        run(f'{prefix}PUSHBEFORE {name} d c')
        run(f'{prefix}PUSHAFTER {name} d e')
        run(f'{prefix}READHT {name}', 'a b c d e')
        run(f'{prefix}READTH {name}', 'e d c b a')
        run(f'{prefix}FINDVAL {name} c', '2')
        run(f'{prefix}FINDVAL {name} absent', '-1')
        run(f'{prefix}POPBEFORE {name} c')
        run(f'{prefix}POPAFTER {name} c')
        run(f'{prefix}POPHEAD {name}')
        run(f'{prefix}POP {name}')
        run(f'{prefix}READHT {name}', 'c')
        run(f'{prefix}DELVAL {name} c')
        run(f'{prefix}READHT {name}', '')
        # Duplicate keys: relative operations target the first match.
        run(f'{prefix}PUSH {name} x y x')
        run(f'{prefix}PUSHAFTER {name} x z')
        run(f'{prefix}READHT {name}', 'x z y x')
        run(f'{prefix}DELVAL {name} x')
        run(f'{prefix}READTH {name}', 'x y z')

    run('SCREATE s')
    run('SPOP s', error=True)
    run('SPUSH s 1 2 3')
    run('SREAD s', '3 2 1')
    before = state.read_bytes()
    run('PRINT s', '=== PRINT s === 3 2 1')
    assert state.read_bytes() == before
    for item in ['3', '2', '1']: run('SPOP s', item)
    run('SPOP s', error=True)
    run('QCREATE q')
    run('QPOP q', error=True)
    run('QPUSH q 1 2 3')
    run('QREAD q', '1 2 3')
    for item in ['1', '2', '3']: run('QPOP q', item)
    run('QPOP q', error=True)

    run('TCREATE t')
    run('TCHECKCOMPLETE t', 'TRUE')
    run('TFIND t 1', 'FALSE')
    run('TINSERT t 10 5 20 3 7 15')
    run('TREAD t', '10 5 20 3 7 15')
    run('TCHECKCOMPLETE t', 'TRUE')
    run('TGET t 7', 'TRUE')
    run('ISMEMBER t 100', 'FALSE')
    run('PRINT t', '''=== PRINT t ===
root = 10
root->left = 5
root->right = 20
root->left->left = 3
root->left->right = 7
root->right->left = 15''')
    run('TINSERT t 20 -2')
    run('TREAD t', '10 5 20 3 7 15 20 -2')
    run('TINSERT t 33 bad 44', error=True)
    run('TFIND t 2147483648', error=True)
    for query in ['PRINT absent', 'TFIND t', 'TCHECKCOMPLETE t extra', 'MCREATE t']:
        run(query, error=True)

    # Model-based tests exercise pointer rewiring, duplicates and boundary conditions.
    rng = random.Random(3)
    for prefix in ['F', 'L']:
        name = prefix + 'random'
        run(f'{prefix}CREATE {name}')
        model = []
        for _ in range(120):
            op = rng.choice(['PUSH', 'PUSHHEAD', 'PUSHBEFORE', 'PUSHAFTER', 'POP', 'POPHEAD', 'POPBEFORE', 'POPAFTER', 'DELVAL'])
            key, value = str(rng.randrange(5)), str(rng.randrange(5))
            args = ''
            if op in ['PUSH', 'PUSHHEAD']:
                args = value
                model.insert(len(model) if op == 'PUSH' else 0, value)
            elif op in ['PUSHBEFORE', 'PUSHAFTER']:
                args = key + ' ' + value
                if key in model: model.insert(model.index(key) + (op == 'PUSHAFTER'), value)
            elif op in ['POP', 'POPHEAD']:
                if model: model.pop(-1 if op == 'POP' else 0)
            else:
                args = key
                if key in model:
                    idx = model.index(key) + {'POPBEFORE': -1, 'POPAFTER': 1, 'DELVAL': 0}[op]
                    if 0 <= idx < len(model): model.pop(idx)
            run(f'{prefix}{op} {name} {args}')
            run(f'{prefix}READHT {name}', ' '.join(model))
            run(f'{prefix}READTH {name}', ' '.join(reversed(model)))
    # Growth beyond initial capacity and a larger complete tree.
    run('MCREATE big')
    run('MPUSH big ' + ' '.join(map(str, range(100))))
    run('MGET big 99', '99')
    run('TINSERT large ' + ' '.join(map(str, range(150))))
    run('TCHECKCOMPLETE large', 'TRUE')
    run('TFIND large 149', 'TRUE')
    run('TREAD large', ' '.join(map(str, range(150))))

    # Malformed persisted state must never be silently discarded.
    for bad in ['Unknown x 1\n', 'Tree broken 1 nope\n', 'Array same 1\nQueue same 2\n', 'Array\n']:
        state.write_text(bad)
        run('MCREATE new', error=True)
print(f'PASS: {checks} end-to-end checks ({binary.name})')

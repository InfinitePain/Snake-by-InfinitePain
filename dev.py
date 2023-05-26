import argparse
import json
import os
import subprocess
import sys

def check_preset(preset):
    with open('CMakePresets.json') as f:
        presets = json.load(f)
    if preset not in [preset['name'] for preset in presets['presets']]:
        raise ValueError(f"Preset '{preset}' not found in CMakePresets.json")

def configure(preset):
    if preset == 'default':
        subprocess.run(['cmake', '-S', '.', '-B', './out/build/default', '-DBUILD_TESTING=ON'])
    else:
        subprocess.run(['cmake', '-S', '.', '-B', f'./out/build/{preset}', '--preset', preset])

def build(preset):
    if preset == 'default':
        subprocess.run(['cmake', '--build', './out/build/default'])
    else:
        subprocess.run(['cmake', '--build', f'./out/build/{preset}', '--preset', preset])

def test(preset):
    if preset == 'default':
        folder = './out/build/default'
    else:
        folder = f'./out/build/{preset}'
    test_count = int(subprocess.check_output(['ctest', '-N', '-C', folder]).decode().split('Total Tests: ')[1].split('\n')[0])
    if test_count == 0:
        print("No tests available. Continuing...")
    else:
        subprocess.run(['ctest', '-C', folder])

def run(preset):
    if preset == 'default':
        subprocess.run(['./out/build/default/Snake-by-InfinitePain/Snake-by-InfinitePain'])
    else:
        subprocess.run([f'./out/build/{preset}/Snake-by-InfinitePain/Snake-by-InfinitePain'])

def version(version, force):
    if force:
        subprocess.run(['python', 'update_version.py', version, '--force'])
    else:
        subprocess.run(['python', 'update_version.py', version])

def clean(preset):
    if preset == 'default':
        subprocess.run(['rm', '-rf', './out/build/default'])
    else:
        subprocess.run(['rm', '-rf', f'./out/build/{preset}'])

def run_command(command, preset):
    if preset == 'none':
        result = subprocess.run(command.split())
    else:
        result = subprocess.run((command + f' --preset {preset}').split())
    if result.returncode != 0:
        sys.exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description='Python script equivalent of the Makefile for the Snake-by-InfinitePain project.')
    parser.add_argument('commands', nargs='+', choices=['configure', 'build', 'test', 'run', 'version', 'clean'], help='The commands to run.')
    parser.add_argument('--preset', default='none', help='The preset to use from CMakePresets.json.')
    parser.add_argument('--version', default='none', help='The new version for the version command.')
    parser.add_argument('--force', action='store_true', help='Force the version update even if the specified version is invalid or lower than the current version.')
    args = parser.parse_args()

    if 'clean' in args.commands and len(args.commands) > 1:
        print("Error: The 'clean' command cannot be used with other commands")
        sys.exit(1)

    if 'version' in args.commands:
        if args.version == 'none':
            print("Error: No version specified")
            sys.exit(1)
        if args.force:
            subprocess.run(['python', 'update_version.py', args.version, '--force'])
        else:
            subprocess.run(['python', 'update_version.py', args.version])

    if 'configure' in args.commands:
        run_command(f'cmake -S . -B ./out/build/{args.preset}', args.preset)

    if 'build' in args.commands:
        run_command(f'cmake --build ./out/build/{args.preset}', args.preset)

    if 'test' in args.commands:
        run_command(f'ctest -C ./out/build/{args.preset}', args.preset)

    if 'run' in args.commands:
        run_command(f'./out/build/{args.preset}/Snake-by-InfinitePain/Snake-by-InfinitePain', args.preset)

    if 'clean' in args.commands:
        subprocess.run(['rm', '-rf', f'./out/build/{args.preset}'])

if __name__ == '__main__':
    main()
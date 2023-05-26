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
    if preset == None:
        result = subprocess.run(['cmake', '-S', '.', '-B', './out/build/default', '-DBUILD_TESTING=ON'])
    else:
        result = subprocess.run(['cmake', '-S', '.', '-B', f'./out/build/{preset}', '--preset', preset])
    if result.returncode != 0:
        sys.exit(result.returncode)

def build(preset):
    if preset == None:
        result = subprocess.run(['cmake', '--build', './out/build/default'])
    else:
        result = subprocess.run(['cmake', '--build', f'./out/build/{preset}', '--preset', preset])
    if result.returncode != 0:	
        sys.exit(result.returncode)

def test(preset):
    if preset == None:
        folder = './out/build/default'
    else:
        folder = f'./out/build/{preset}'
    test_count = int(subprocess.check_output(['ctest', '-N', '-C', folder]).decode().split('Total Tests: ')[1].split('\n')[0])
    if test_count == 0:
        print("No tests available. Continuing...")
    else:
        result = subprocess.run(['ctest', '-C', folder])
    if result.returncode != 0:
        sys.exit(result.returncode)

def run(preset):
    if preset == None:
        result = subprocess.run(['./out/build/default/Snake-by-InfinitePain/Snake-by-InfinitePain'])
    else:
        result = subprocess.run([f'./out/build/{preset}/Snake-by-InfinitePain/Snake-by-InfinitePain'])
    if result.returncode != 0:
        sys.exit(result.returncode)

def version(version, force):
    if version == None:
        print("Error: No version specified")
        sys.exit(1)
    if force:
        subprocess.run(['python', 'update_version.py', version, '--force'])
    else:
        subprocess.run(['python', 'update_version.py', version])

def clean(preset, all):
    if all and preset != None:
        print("Error: Both 'all' flag and preset are specified. Please specify only one.")
        sys.exit(1)
    elif all:
        subprocess.run(['rm', '-rf', './out/build/*'])
    elif preset == None:
        print("Error: No preset specified")
        sys.exit(1)
    else:
        if not os.path.exists(f'./out/build/{preset}'):
            print(f"Error: Preset '{preset}' does not exist")
            sys.exit(1)
        subprocess.run(['rm', '-rf', f'./out/build/{preset}'])

def main():
    parser = argparse.ArgumentParser(description='Python script equivalent of the Makefile for the Snake-by-InfinitePain project.')
    parser.add_argument('commands', nargs='+', choices=['configure', 'build', 'test', 'run', 'version', 'clean'], help='The commands to run.')
    parser.add_argument('--preset', default=None, help='The preset to use from CMakePresets.json.')
    parser.add_argument('--version', default=None, help='The new version for the version command.')
    parser.add_argument('--force', action='store_true', help='Force the version update even if the specified version is invalid or lower than the current version.')
    parser.add_argument('--all', action='store_true', help='Delete all presets when cleaning.')
    args = parser.parse_args()

    if 'clean' in args.commands and len(args.commands) > 1:
        print("Error: The 'clean' command cannot be used with other commands")
        sys.exit(1)

    if 'version' in args.commands:
        version(args.version, args.force)

    if 'configure' in args.commands:
        configure(args.preset)

    if 'build' in args.commands:
        build(args.preset)

    if 'test' in args.commands:
        test(args.preset)

    if 'run' in args.commands:
        run(args.preset)

    if 'clean' in args.commands:
        clean(args.preset, args.all)

if __name__ == '__main__':
    main()
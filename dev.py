import argparse
import json
import os
import subprocess
import sys
import select    
import time

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

def read_input():
    timeout = 30  # seconds
    print("Tests failed. Would you like to continue anyway? [y/N]")
    if os.name == 'nt':  # Windows
        import msvcrt
        start_time = time.time()
        while True:
            if msvcrt.kbhit():
                user_input = sys.stdin.readline().strip()
                return user_input
            elif time.time() - start_time > timeout:
                print("Time out. Exiting due to test failure.")
                sys.exit(1)
            time.sleep(0.1)
    else:  # Unix-based
        i, o, e = select.select( [sys.stdin], [], [], timeout )
        if (i):
            user_input = sys.stdin.readline().strip()
            return user_input
        else:
            print("Time out. Exiting due to test failure.")
            sys.exit(1)


def test(preset):
    if preset is None:
        folder = './out/build/default'
        os.chdir(folder)
        result = subprocess.run(['ctest', '-N'], capture_output=True, text=True)
    else:
        result = subprocess.run(['ctest', '--preset', preset, '-N'], capture_output=True, text=True)
        
    if "No such test preset" in result.stderr:
        print("No tests available. Continuing...")
        return
    else:
        result = subprocess.run(['ctest', '--preset', preset], capture_output=True, text=True)
        print(result.stdout)

    if result.returncode != 0:
        user_input = read_input()
        if user_input.lower() == 'y':
            print("Continuing despite test failure.")
        else:
            print("Exiting due to test failure.")
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

def installer():
    configure('Windows-x64-Release')
    build('Windows-x64-Release')

    os.chdir('./out/build/Windows-x64-Release')

    result = subprocess.run(['cpack'])
    if result.returncode != 0:
        sys.exit(result.returncode)

    os.chdir('../../../installers/wix')

    result = subprocess.run(['candle', '-ext', 'WixBalExtension', '-ext', 'WixUtilExtension', 'bundle.wxs'])
    if result.returncode != 0:
        sys.exit(result.returncode)

    result = subprocess.run(['light', '-ext', 'WixBalExtension', '-ext', 'WixUtilExtension', 'Bundle.wixobj', '-o', 'Snake-by-InfinitePain-Windows_Installer.exe'])
    if result.returncode != 0:
        sys.exit(result.returncode)

class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    pass

def main():
    parser = argparse.ArgumentParser(description='Python script for the Snake-by-InfinitePain project.')
    parser = argparse.ArgumentParser(description='''Python script for the Snake-by-InfinitePain project.

Commands:
  configure           Configure the project using a preset from CMakePresets.json.
  build               Build the project using a preset from CMakePresets.json.
  test                Run tests on the project using a preset from CMakePresets.json.
  run                 Run the project using a preset from CMakePresets.json.
  version             Update the version of the project.
  clean               Clean the build directory of the project.
  installer           Create an installer for the project.''', formatter_class=CustomFormatter)
    parser.add_argument('commands', nargs='+', help=argparse.SUPPRESS)
    parser.add_argument('--preset', default=None, help='The preset to use from CMakePresets.json.')
    parser.add_argument('--version', default=None, help='The new version for the version command.')
    parser.add_argument('--force', action='store_true', help='Force the version update even if the specified version is invalid or lower than the current version.')
    parser.add_argument('--all', action='store_true', help='Delete all presets when cleaning.')
    args = parser.parse_args()

    if ('clean' in args.commands or 'installer' in args.commands) and len(args.commands) > 1:
        print("Error: The 'clean' and 'installer' commands cannot be used with other commands")
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

    if 'installer' in args.commands:
        installer()

if __name__ == '__main__':
    main()
import sys
import os
import re
import argparse
import time

def compare_versions(version1, version2):
    v1_parts = [int(x) for x in version1.split('.')]
    v2_parts = [int(x) for x in version2.split('.')]

    for v1, v2 in zip(v1_parts, v2_parts):
        if v1 < v2:
            return -1
        elif v1 > v2:
            return 1
    return 0

def update_version(version, force=False):
    if not re.match(r'^\d+\.\d+\.\d+$', version):
        if force:
            print("Warning: Version number does not match the required pattern (x.x.x). Proceeding with the force option.")
        else:
            print("Error: Version number does not match the required pattern (x.x.x). Use '--force' to override this check.")
            sys.exit(1)

    file_paths = [
        ('Installers/wix/Bundle.wxs', r'(<\?define ProductVersion = ")([^"]*)("\s*\?>)'),
        ('src/snakebyip.c', r'(\s*Version\s*:\s*)([^*\s]*)(\s*)'),
        ('CMakeLists.txt', r'(project\(Snake-by-InfinitePain VERSION )([^\)]*)(\))'),
        ('installers/arch_linux/PKGBUILD', r'(pkgver=)([^*\s]*)(\s*)'),
    ]

    for file_path, pattern in file_paths:
        with open(file_path, 'r') as file:
            content = file.read()

        current_version = re.search(pattern, content).group(2)

        if compare_versions(version, current_version) < 0:
            if force:
                print("Warning: The new version is lower than the current version. Proceeding with the force option.")
            else:
                print("Error: The new version is lower than the current version. Use '--force' to override this check.")
                sys.exit(1)

        new_content = re.sub(pattern, r'\g<1>' + version + r'\g<3>', content)

        with open(file_path, 'w') as file:
            file.write(new_content)

def get_user_input(prompt, timeout=30):
    start_time = time.time()
    print(prompt, end='', flush=True)

    if os.name == 'nt':  # Windows
        import msvcrt
        start_time = time.time()
        while True:
            if msvcrt.kbhit():
                user_input = sys.stdin.readline().strip()
                return user_input
            elif time.time() - start_time > timeout:
                return ''
            time.sleep(0.1)
    else:  # Unix-based
        i, o, e = select.select( [sys.stdin], [], [], timeout )
        if (i):
            user_input = sys.stdin.readline().strip()
            return user_input
        else:
            return ''
            

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Update version script')
    parser.add_argument('version', help='The new version number (format: x.x.x)')
    parser.add_argument('--force', action='store_true', help='Force update even if the version number does not match the required pattern or is lower than the current version')
    args = parser.parse_args()

    if args.force and not re.match(r'^\d+\.\d+\.\d+$', args.version):
        print("Warning: This may cause issues with future updates. Make sure to revert the version number to a valid format before updating again.")
        confirmation = get_user_input("Type 'FORCE' within 60 seconds to proceed: ")

        if confirmation != "FORCE":
            print("Operation canceled. Please try again with a valid version number or type 'FORCE' to proceed.")
            sys.exit(1)

    update_version(args.version, args.force)
    print(f"Updated version to: {args.version}")

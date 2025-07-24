import subprocess
import argparse
import os

def init_default_key_vector():
    """Initialize the default key vector (a-z, 0-9, A-Z)."""
    key_vector = list('abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ')
    return key_vector

def decrypt(encrypted, sPos, key_vector):
    """Decrypt a BFE-encrypted string using the starting position and key vector."""
    if not key_vector:
        key_vector = init_default_key_vector()
    
    key_size = len(key_vector)
    current_pos = sPos
    decrypted = []
    
    for char in encrypted:
        if char == '>':
            current_pos = (current_pos + 1) % key_size
        elif char == '+':
            decrypted.append(key_vector[current_pos])
    
    return ''.join(decrypted)

def encrypt(message, sPos=None, kvec=None):
    """Encrypt a message using the bfe binary."""
    # Construct the command
    cmd = ['./bfe', '--message', message]
    
    if sPos is not None:
        cmd.extend(['--spos', str(sPos)])
    if kvec is not None:
        cmd.extend(['--kvec', kvec])
    
    try:
        # Run the bfe binary and capture output
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=True
        )
        # Extract the encrypted output from the binary's stdout
        output_lines = result.stdout.splitlines()
        encrypted = None
        used_spos = None
        used_kvec = None
        
        for line in output_lines:
            if line.startswith('Encrypted Output:'):
                encrypted = line.split(': ')[1]
            elif line.startswith('Starting Position:'):
                used_spos = int(line.split(': ')[1].split(' ')[0])
            elif line.startswith('Key Vector:'):
                used_kvec = line.split(': ')[1]
                if used_kvec.startswith('['):
                    used_kvec = ''.join(init_default_key_vector())
        
        return encrypted, used_spos, used_kvec
    except subprocess.CalledProcessError as e:
        print(f"Error running bfe binary: {e.stderr}")
        return None, None, None
    except FileNotFoundError:
        print("Error: bfe binary not found. Ensure it is compiled and in the same directory or PATH.")
        return None, None, None

def main():
    parser = argparse.ArgumentParser(
        description="BrainFuckEncryption (BFE) Client - Encrypt and decrypt messages using the bfe binary.",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument('--message', required=True, help='Message to encrypt')
    parser.add_argument('--spos', type=int, help='Starting position in key vector (default: random)')
    parser.add_argument('--kvec', help='Custom key vector (default: random or a-z,0-9,A-Z)')
    parser.add_argument('--decrypt', action='store_true', help='Decrypt the provided message instead of encrypting')

    args = parser.parse_args()

    if args.decrypt:
        # Decrypt mode
        if args.spos is None or args.kvec is None:
            print("Error: --spos and --kvec are required for decryption")
            return
        decrypted = decrypt(args.message, args.spos, list(args.kvec) if args.kvec else None)
        print("\nInput (Encrypted):", args.message)
        print("Starting Position:", args.spos)
        print("Key Vector:", args.kvec)
        print("Decrypted Output:", decrypted, "\n")
    else:
        # Encrypt mode
        encrypted, used_spos, used_kvec = encrypt(args.message, args.spos, args.kvec)
        if encrypted:
            print("\nInput Message:", args.message)
            print("Starting Position:", used_spos)
            print("Key Vector:", used_kvec)
            print("Encrypted Output:", encrypted, "\n")
            # Verify decryption
            decrypted = decrypt(encrypted, used_spos, list(used_kvec) if used_kvec else None)
            print("Verification (Decrypted):", decrypted, "\n")

if __name__ == "__main__":
    main()
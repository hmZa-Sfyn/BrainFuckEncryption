import subprocess
import argparse
import os

def init_default_key_vector():
    """Initialize the default key vector (a-z, 0-9, A-Z, space)."""
    return list('abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ')

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
        elif char == '<':
            current_pos = (current_pos - 1) % key_size
        elif char == '+':
            decrypted.append(key_vector[current_pos])
    
    return ''.join(decrypted)

def compress(encrypted):
    """Compress an encrypted string using the bfe_compress binary."""
    try:
        result = subprocess.run(
            ['bfe_compress', '--input', encrypted, '--compress'],
            capture_output=True,
            text=True,
            check=True
        )
        output_lines = result.stdout.splitlines()
        for line in output_lines:
            if line.startswith('Output:'):
                return line.split(': ')[1]
        return None
    except subprocess.CalledProcessError as e:
        print(f"Error running bfe_compress: {e.stderr}")
        return None
    except FileNotFoundError:
        print("Error: bfe_compress binary not found. Ensure it is compiled and in the same directory or PATH.")
        return None

def decompress(compressed):
    """Decompress a compressed string using the bfe_compress binary."""
    try:
        result = subprocess.run(
            ['bfe_compress', '--input', compressed, '--decompress'],
            capture_output=True,
            text=True,
            check=True
        )
        output_lines = result.stdout.splitlines()
        for line in output_lines:
            if line.startswith('Output:'):
                return line.split(': ')[1]
        return None
    except subprocess.CalledProcessError as e:
        print(f"Error running bfe_compress: {e.stderr}")
        return None
    except FileNotFoundError:
        print("Error: bfe_compress binary not found. Ensure it is compiled and in the same directory or PATH.")
        return None

def encrypt(message, sPos=None, kvec=None):
    """Encrypt a message using the bfe binary."""
    cmd = ['bfe', '--message', message]
    
    if sPos is not None:
        cmd.extend(['--spos', str(sPos)])
    if kvec is not None:
        cmd.extend(['--kvec', kvec])
    
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=True
        )
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
        description="BrainFuckEncryption CLI (bfecli) - Encrypt/compress and decompress/decrypt messages.",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument('--message', required=True, help='Message to encrypt/compress or decrypt/decompress')
    parser.add_argument('--spos', type=int, help='Starting position in key vector (default: random for encrypt, required for decrypt)')
    parser.add_argument('--kvec', help='Custom key vector (default: random or a-z,0-9,A-Z,space for encrypt, required for decrypt)')
    parser.add_argument('--encrypt', action='store_true', help='Encrypt and optionally compress the message')
    parser.add_argument('--decrypt', action='store_true', help='Decompress and decrypt the message')
    parser.add_argument('--compress', action='store_true', help='Compress the encrypted output (used with --encrypt)')

    args = parser.parse_args()

    if (args.encrypt + args.decrypt) != 1:
        print("Error: Exactly one of --encrypt or --decrypt must be specified")
        return

    if args.decrypt:
        # Decompress and decrypt mode
        if args.spos is None or args.kvec is None:
            print("Error: --spos and --kvec are required for decryption")
            return
        decompressed = decompress(args.message)
        if not decompressed:
            return
        decrypted = decrypt(decompressed, args.spos, list(args.kvec) if args.kvec else None)
        print("\nInput (Compressed):", args.message)
        print("Decompressed:", decompressed)
        print("Starting Position:", args.spos)
        print("Key Vector:", args.kvec)
        print("Decrypted Output:", decrypted, "\n")
    else:
        # Encrypt and optionally compress mode
        encrypted, used_spos, used_kvec = encrypt(args.message, args.spos, args.kvec)
        if not encrypted:
            return
        if args.compress:
            compressed = compress(encrypted)
            if not compressed:
                return
            print("\nInput Message:", args.message)
            print("Starting Position:", used_spos)
            print("Key Vector:", used_kvec)
            print("Encrypted Output:", encrypted)
            print("Compressed Output:", compressed, "\n")
            # Verify by decompressing and decrypting
            decompressed = decompress(compressed)
            if decompressed:
                decrypted = decrypt(decompressed, used_spos, list(used_kvec) if used_kvec else None)
                print("Verification (Decompressed):", decompressed)
                print("Verification (Decrypted):", decrypted, "\n")
        else:
            print("\nInput Message:", args.message)
            print("Starting Position:", used_spos)
            print("Key Vector:", used_kvec)
            print("Encrypted Output:", encrypted, "\n")
            # Verify by decrypting
            decrypted = decrypt(encrypted, used_spos, list(used_kvec) if used_kvec else None)
            print("Verification (Decrypted):", decrypted, "\n")

if __name__ == "__main__":
    main()
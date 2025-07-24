# 🧠 BrainFuckEncryption (BFE)

<code>BrainFuckEncryption (BFE)</code> is a C-based encryption system that converts text into a sequence of <code>'>'</code> (forward movement), <code>'<'</code> (backward movement), and <code>'+'</code> (character selection) using a customizable key vector. It includes a compression tool to reduce the size of the encrypted output and a Python client for easy encryption and decryption.

---

## ✨ Features

- <code>bfe.c</code>: Encrypts text using a key vector (default: <code>a-z</code>, <code>0-9</code>, <code>A-Z</code>, space) and a starting position (<code>sPos</code>).  
  Uses the shortest path (<code>'>'</code> or <code>'<'</code>) to reach each character.
- <code>bfe_compress.c</code>: Compresses encrypted output (e.g., <code>>>>+<<+</code> to <code>[4]>+[2]<+</code>) and decompresses back.
- <code>bfe_client.py</code>: Python interface to the <code>bfe</code> binary for encryption and includes a decryption function.
- Command-line interface with flags:
  - <code>--message</code>, <code>--spos</code>, <code>--kvec</code> for encryption  
  - <code>--input</code>, <code>--compress</code>, <code>--decompress</code> for compression
- Random generation of <code>sPos</code> and key vector if not provided.
- Supports large messages (up to 1MB).

---

## 🛠️ Installation

### Requirements

- <code>C Compiler</code>: GCC (e.g., via MinGW or MSYS2 on Windows)
- <code>Python 3</code> (standard libraries only)
- <code>Git</code> (for cloning)

---

### Setup

**Clone the Repository**
<pre><code>git clone https://github.com/your-username/BrainFuckEncryption.git
cd BrainFuckEncryption
</code></pre>

**Compile the C Programs**  
Ensure GCC is installed:
<pre><code>gcc src/bfe.c -o bfe
gcc src/bfe_compress.c -o bfe_compress
</code></pre>

**Verify Python Installation**
<pre><code>python --version
</code></pre>

---

## 🚀 Usage

### 🔐 Encryption (<code>bfe</code>)

Encrypt a message with optional starting position and key vector:
<pre><code>./bfe --message "hello world"
</code></pre>

**Sample Output (random <code>sPos</code> and <code>kvec</code>):**
<pre><code>Input Message: hello world
Starting Position: 5 (f)
Key Vector: xy123zABCD
Encrypted Output: >>>>>+<<<<+...<<+
</code></pre>

With specific <code>sPos</code> and custom key vector:
<pre><code>./bfe --message "hello" --spos 0 --kvec "abc123"
</code></pre>

---

### 📦 Compression/Decompression (<code>bfe_compress</code>)

Compress an encrypted string:
<pre><code>./bfe_compress --input ">>>>+<<+" --compress
</code></pre>

**Output:**
<pre><code>Input: >>>>+<<+
Output: [4]>+[2]<+
</code></pre>

Decompress back:
<pre><code>./bfe_compress --input "[4]>+[2]<+" --decompress
</code></pre>

**Output:**
<pre><code>Input: [4]>+[2]<+
Output: >>>>+<<+
</code></pre>

---

### 🐍 Python Client (<code>bfe_client.py</code>)

Encrypt a message:
<pre><code>python bfe_client.py --message "hello world"
</code></pre>

Decrypt an encrypted string (requires <code>sPos</code> and <code>kvec</code>):
<pre><code>python bfe_client.py --decrypt --message ">>>>+<<+" --spos 0 --kvec "abc123"
</code></pre>

---

## ⚙️ How It Works

- **Encryption**:  
  Maps each character in the message to its position in the key vector.  
  Moves the pointer using <code>'>'</code> or <code>'<'</code> to minimize steps, and marks character selections with <code>'+'</code>.

- **Compression**:  
  Converts sequences of <code>'>'</code> or <code>'<'</code> into <code>[count]></code> or <code>[count]<</code>, preserving <code>'+'</code>.

- **Decryption**:  
  Reverses encryption by tracking pointer movements and collecting characters where <code>'+'</code> appears.

---

## 📝 Notes

- Default key vector includes: lowercase letters, digits, uppercase letters, and spaces.
- Encryption is **deterministic** if the same <code>sPos</code> and <code>key vector</code> are used.
- Ensure <code>bfe</code> binary is in the same directory as <code>bfe_client.py</code> or in your <code>$PATH</code>.
- Optimized for memory use with **dynamic allocation**, even for large messages.

---

## 🤝 Contributing

Feel free to open <code>issues</code> or submit <code>pull requests</code> via the GitHub repository!

---

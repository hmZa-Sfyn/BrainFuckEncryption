# 🧠 BrainFuckEncryption (BFE)

<code>BrainFuckEncryption (BFE)</code> is a C-based encryption system that converts text into a sequence of <code>'>'</code> (forward movement), <code>'<'</code> (backward movement), and <code>'+'</code> (character selection) using a customizable key vector. It includes a compression tool to reduce the size of the encrypted output and Python clients for encryption, decryption, compression, and decompression.

---

## ✨ Features

- <code>bfe.c</code>: Encrypts text using a key vector (default: <code>a-z</code>, <code>0-9</code>, <code>A-Z</code>, space) and a starting position (<code>sPos</code>).  
  Supports spaces and uses the shortest path (forward with <code>'>'</code> or backward with <code>'<'</code>) to each character.
- <code>bfe_compress.c</code>: Compresses encrypted output (e.g., <code>>>>+<<+</code> to <code>[4]>+[2]<+</code>) and decompresses back to the original format.
- <code>bfe_client.py</code>: A Python script for encryption and decryption using the <code>bfe</code> binary.
- <code>bfecli.py</code>: A Python script that combines encryption/compression and decompression/decryption using both <code>bfe</code> and <code>bfe_compress</code> binaries.
- Command-line interface with flags:
  - <code>--message</code>, <code>--spos</code>, <code>--kvec</code> for encryption
  - <code>--input</code>, <code>--compress</code>, <code>--decompress</code> for compression
  - <code>--encrypt</code>, <code>--decrypt</code> for <code>bfecli</code>
- Random generation of <code>sPos</code> and <code>key vector</code> if not provided.
- Supports large messages (up to 1MB).

---

## 🛠 Installation

### Requirements

- <code>C Compiler</code>: GCC (e.g., via MinGW or MSYS2 on Windows)  
- <code>Python 3</code>: For running Python scripts  
- <code>Git</code>: For version control

---

### Setup

**Clone the Repository:**
<pre><code>git clone https://github.com/your-username/BrainFuckEncryption.git
cd BrainFuckEncryption
</code></pre>

**Compile the C Programs:**  
Ensure GCC is installed.
<pre><code>gcc src/bfe.c -o bfe
gcc src/bfe_compress.c -o bfe_compress
</code></pre>

**Verify Python Installation:**
<pre><code>python --version
</code></pre>

---

## 🚀 Usage

### 🔐 Encryption (<code>bfe</code>)

Encrypt a message with optional starting position and key vector:
<pre><code>./bfe --message "hello world"
</code></pre>

With specific <code>sPos</code> and custom key vector:
<pre><code>./bfe --message "hello" --spos 0 --kvec "abc123 "
</code></pre>

---

### 📦 Compression/Decompression (<code>bfe_compress</code>)

Compress an encrypted string:
<pre><code>./bfe_compress --input ">>>>+<<+" --compress
</code></pre>

Decompress back:
<pre><code>./bfe_compress --input "[4]>+[2]<+" --decompress
</code></pre>

---

### 🐍 Python Client (<code>bfe_client.py</code>)

Encrypt a message:
<pre><code>python bfe_client.py --message "hello world"
</code></pre>

Decrypt an encrypted string:
<pre><code>python bfe_client.py --decrypt --message ">>>>+<<+" --spos 0 --kvec "abc123 "
</code></pre>

---

### 🧪 Integrated CLI (<code>bfecli.py</code>)

Encrypt and compress:
<pre><code>python bfecli.py --encrypt --message "hello world" --compress
</code></pre>

Decompress and decrypt:
<pre><code>python bfecli.py --decrypt --message "[4]>+[2]<+" --spos 0 --kvec "abc123 "
</code></pre>

---

## ⚙️ How It Works

- **Encryption**:  
  Maps each character in the message to a position in the key vector, moves the pointer using <code>'>'</code> or <code>'<'</code> to minimize steps, and uses <code>'+'</code> to select.

- **Compression**:  
  Converts runs of <code>'>'</code> or <code>'<'</code> into compact forms like <code>[4]></code>, preserving <code>'+'</code>.

- **Decryption**:  
  Reverses encryption by tracking pointer movements and character selections.

- **Decompression**:  
  Expands <code>[count]></code> or <code>[count]></code> back into sequences of characters.

---

## 📝 Notes

- Default key vector includes: lowercase letters, digits, uppercase letters, and a space.
- Encryption is **deterministic** given the same <code>sPos</code> and <code>key vector</code>.
- Ensure <code>bfe</code> and <code>bfe_compress</code> binaries are in the same directory as <code>bfecli.py</code> or available in your <code>PATH</code>.
- Dynamic allocation allows handling of very large messages efficiently.

---

## 🤝 Contributing

Feel free to open <code>issues</code> or submit <code>pull requests</code> to improve or extend the project.

---


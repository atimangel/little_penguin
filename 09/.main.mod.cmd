savedcmd_/root/LP/09/main.mod := printf '%s\n'   main.o | awk '!x[$$0]++ { print("/root/LP/09/"$$0) }' > /root/LP/09/main.mod

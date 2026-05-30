savedcmd_translator.mod := printf '%s\n'   translator.o | awk '!x[$$0]++ { print("./"$$0) }' > translator.mod

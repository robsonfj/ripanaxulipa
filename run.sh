#!/bin/sh
# run.sh - roda o jogo no Linux/macOS (use apos buildar).
# Uso: ./run.sh   (a partir da raiz do repo)
set -e
cd "$(dirname "$0")/build" 2>/dev/null || { echo "rode o build antes (ver README)"; exit 1; }
./RipaNaXulipa

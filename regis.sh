#!/bin/bash

T=$1
DEMO2=0

function send() {
  /bin/echo -e -n "$1" > $T
}

function init() {
  send '\x1bP1p'
}

function fini() {
  send '\x1b\\'
}

init

if [ $DEMO2 -eq 1 ]; then
  # https://ilkerf.tripod.com/cdoc/decregis.html
  # https://vt100.net/shuford/terminal/dec_regis_news.txt
  send 'S(E),S(C1),P[100,440]'
  send 'V(S),[+100,+0],[+0,-10],[+0,+10],(E)'
else
  # https://github.com/feilipu/ReGIS?tab=readme-ov-file#demonstration
  send 'S(E)'
  send 'W(I(M))P[600,200]V[][-200,+200]V[][330,100]'
  send 'W(I(G))P[700,100]V(B)[+050,][,+050][-050,](E)' # square
  send 'V(W(S1))(B)[-100,][,-050][+100,](E)' # fancy filled object
  send 'V(W(S1,E))(B)[-050,][,-025][+050,](E)'
  send 'W(I(C))P[200,100]C(A-180)[+100]C(A+180)[+050]' # half circles
  send 'W(I(B))P[250,300]C(W(S1))[+80]C(W(S1,E))[+40]' # filled doughnut
  send 'W(I(W))P[420,150]T(S02)"Hey there!"' # text
fi

fini

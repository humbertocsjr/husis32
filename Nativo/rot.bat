echo == Compilando %1
del %1.rot
\Nativo\osasmcom %1
ren %1.com %1.rot
del %1.lst

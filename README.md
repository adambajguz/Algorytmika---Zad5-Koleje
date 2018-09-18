# Algorytmika---Koleje
Projekt w CLion

README.pdf

Pliki z kodem źródłowym nie zawierają komentarzy (odgórny wymóg).




**Pole `struct t_record *conn_build_record` może zostać usunięte, gydż jego funkcję można zastąpić polem `*conn_last_record`:**

  - zakomentować linię nr 44 w pliku input.c

  - zamienić linie 55 w pliku input.c na: `conn_build_record = edge_to_end->conn_build_record;`

  - usunąć pole `*conn_build_record` w pliku Graph.h

:: É necessário já ter o GCC instalado e no PATH. A implementação de um 'autocompilador' para pessoas que não têm tanto conhecimento sobre tecnologia será revista no futuro, e pretendo que seja possível apenas clicar duas vezes neste arquivo.
gcc -c cadastra_equipamentos.c
gcc cadastra_equipamentos.o Cadastra-Equipamentos.c -o Cadastra-Equipamentos.exe
DEL cadastra_equipamentos.o
START Cadastra-Equipamentos.exe
PAUSE & :: Útil para avisar sobre possíveis erros de compilação, mas a janela do terminal fica aberta. Coloque "pause & " depois dos 4 pontos para fechar a janela automaticamente 

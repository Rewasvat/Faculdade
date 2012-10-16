##################################################
  MAC300 - M�todos Num�ricos da Algebra Linear
  EP1 - Transcri��o Musical
  
  Fernando Omar Aluani
  NUSP: 6797226
##################################################

-----------------------------
COMPILANDO O PROGRAMA:
-----------------------------
Como o programa foi feito em Python, n�o � necess�rio compila��o.

Por�m, para o programa funcionar corretamente � necess�rio que algumas
bibliotecas de Python estejam instaladas. S�o elas:
-NumPy: biblioteca para c�lculo n�merico eficiente em Python, muito parecida com MATLAB.
-matplotlib: biblioteca para cria��o de gr�ficos e visualiza��o de dados.
             Instala��o dessa biblioteca � opcional, mas sem ela os gr�ficos da onda n�o
             ser�o mostrados.
             
O programa tamb�m usa uma biblioteca chamada smidi para cria��o dos arquivos MIDI,
por�m por se tratar de uma biblioteca livre e open-source de um �nico arquivo, inclui
ela com o EP.

O programa foi testado na rede-linux com sucesso - NumPy e matplotlib est�o instalados l�.


-----------------------------
EXECUTANDO O PROGRAMA:
-----------------------------
Para executar o programa, basta rodar no terminal:
    ./EP1 <arquivo_wav> [op��es]
    
Onde arquivo_wav � o caminho/nome do arquivo WAVE (extens�o .wav) que ser� transcrito.
E op��es s�o qualquer combina��o dos seguintes argumentos opcionais:
    -nographs: desativa a cria��o e display dos gr�ficos da onda (em dominio tempo e em
               dominio frequ�ncia) no final da execu��o.
    -dft:METODO: define qual m�todo de DFT ser� usado. M�TODO pode ser:
        matrix: m�todo ing�nuo de multiplica��o pela matriz DFT.
        matrixnumpy: outra forma de multiplica��o pela matriz DFT.
        fft: fast-fourier transform
        both: aplica o 'matrix' e o 'fft', para que se possa comparar os tempos.
        bothfast: aplica o 'matrixnumpy' e o 'fft', para que se possa comparar os tempos.
        all: aplica os tr�s m�todos ('matrix', 'matrixnumpy' e 'fft'), para que se possa
             comparar os tempos.
             
    Para mais informa��es sobre os m�todos DFT, consulte o relat�rio.
    
Ao longo da execu��o, o programa ir� imprimir algumas coisas no terminal, mais notavelmente
os resultados da medi��o do tempo que levou para executar os m�todos DFT.
No final da execu��o, o programa ir� usar as informa��es das notas musicais do arquivo .wav passado
adquiridas com a an�lise do som para criar um arquivo *.MIDI, com o mesmo nome do arquivo de som
original na pasta onde o EP est� sendo executado. Ap�s isso, caso a op��o '-nographs' n�o foi passada,
e a biblioteca matplotlib est� instalada, ele ir� abrir uma janela � parte para mostrar os gr�ficos
de onda (esta janela n�o � aberta em segundo plano - ela precisa ser fechada para terminar a execu��o).
    
Exemplo, o seguinte ir� transcrever o arquivo 'sons/teste.wav', rodando somente por FFT, sem mostrar
os gr�ficos de onda, salvando o resultado como 'teste.MIDI':
    ./EP1 sons/teste.wav -dft:fft -nographs
    
      
-----------------------------
ESTRUTURA DO PROGRAMA:
-----------------------------
O EP est� dividido em 3 scripts python que eu escrevi, e uma pasta com 2 scripts python da biblioteca smidi.
Os 3 scripts python s�o:
EP1.py, que � o script principal, contendo o c�digo para execu��o do programa e an�lise do som.
DFT.py, que cont�m as implementa��es dos m�todos de DFT e medi��o de tempo dos mesmos.
FileUtils.py, que cont�m o c�digo auxiliar para leitura de arquivos .wav e escrita de arquivos .MIDI
##################################################
  MAC300 - Métodos Numéricos da Algebra Linear
  EP1 - Transcrição Musical
  
  Fernando Omar Aluani
  NUSP: 6797226
##################################################

-----------------------------
COMPILANDO O PROGRAMA:
-----------------------------
Como o programa foi feito em Python, não é necessário compilação.

Porém, para o programa funcionar corretamente é necessário que algumas
bibliotecas de Python estejam instaladas. São elas:
-NumPy: biblioteca para cálculo númerico eficiente em Python, muito parecida com MATLAB.
-matplotlib: biblioteca para criação de gráficos e visualização de dados.
             Instalação dessa biblioteca é opcional, mas sem ela os gráficos da onda não
             serão mostrados.
             
O programa também usa uma biblioteca chamada smidi para criação dos arquivos MIDI,
porém por se tratar de uma biblioteca livre e open-source de um único arquivo, inclui
ela com o EP.

O programa foi testado na rede-linux com sucesso - NumPy e matplotlib estão instalados lá.


-----------------------------
EXECUTANDO O PROGRAMA:
-----------------------------
Para executar o programa, basta rodar no terminal:
    ./EP1 <arquivo_wav> [opções]
    
Onde arquivo_wav é o caminho/nome do arquivo WAVE (extensão .wav) que será transcrito.
E opções são qualquer combinação dos seguintes argumentos opcionais:
    -nographs: desativa a criação e display dos gráficos da onda (em dominio tempo e em
               dominio frequência) no final da execução.
    -dft:METODO: define qual método de DFT será usado. MÉTODO pode ser:
        matrix: método ingênuo de multiplicação pela matriz DFT.
        matrixnumpy: outra forma de multiplicação pela matriz DFT.
        fft: fast-fourier transform
        both: aplica o 'matrix' e o 'fft', para que se possa comparar os tempos.
        bothfast: aplica o 'matrixnumpy' e o 'fft', para que se possa comparar os tempos.
        all: aplica os três métodos ('matrix', 'matrixnumpy' e 'fft'), para que se possa
             comparar os tempos.
             
    Para mais informações sobre os métodos DFT, consulte o relatório.
    
Ao longo da execução, o programa irá imprimir algumas coisas no terminal, mais notavelmente
os resultados da medição do tempo que levou para executar os métodos DFT.
No final da execução, o programa irá usar as informações das notas musicais do arquivo .wav passado
adquiridas com a análise do som para criar um arquivo *.MIDI, com o mesmo nome do arquivo de som
original na pasta onde o EP está sendo executado. Após isso, caso a opção '-nographs' não foi passada,
e a biblioteca matplotlib está instalada, ele irá abrir uma janela à parte para mostrar os gráficos
de onda (esta janela não é aberta em segundo plano - ela precisa ser fechada para terminar a execução).
    
Exemplo, o seguinte irá transcrever o arquivo 'sons/teste.wav', rodando somente por FFT, sem mostrar
os gráficos de onda, salvando o resultado como 'teste.MIDI':
    ./EP1 sons/teste.wav -dft:fft -nographs
    
      
-----------------------------
ESTRUTURA DO PROGRAMA:
-----------------------------
O EP está dividido em 3 scripts python que eu escrevi, e uma pasta com 2 scripts python da biblioteca smidi.
Os 3 scripts python são:
EP1.py, que é o script principal, contendo o código para execução do programa e análise do som.
DFT.py, que contém as implementações dos métodos de DFT e medição de tempo dos mesmos.
FileUtils.py, que contém o código auxiliar para leitura de arquivos .wav e escrita de arquivos .MIDI
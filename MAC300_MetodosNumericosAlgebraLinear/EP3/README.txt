##################################################
  MAC300 - Métodos Numéricos da Algebra Linear
  EP3 - Compressão de Imagens usando SVD
  
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
-SciPy: biblioteca para cálculos científicos em Python, usa o NumPy.
-PIL: biblioteca para manipulação de imagens em Python.
             

O programa foi testado na rede-linux com sucesso - NumPy, SciPy e PIL estão instalados lá.

A abertura e salvamento de imagens neste EP foi feita usando funções auxiliares do SciPy, que
internamente usam a biblioteca PIL. A imagem em si é retratada como um array bi-dimensional do NumPy.


-----------------------------
EXECUTANDO O PROGRAMA:
-----------------------------
Para executar o programa, basta rodar no terminal:
    ./EP3.py <rank> <nome_do_arquivo>
    
Onde: 
    rank: é o valor a ser usado como rank na compressão. Quanto menor o valor, mais compressa
          será a imagem e consequente mais detalhes serão perdidos. O rank precisa ser menor que
          min(largura da imagem, altura da imagem)

    nome_do_arquivo: é o nome do arquivo de imagem a ser processado. Vários formatos incluindo
                      jpg e png são aceitos, e a imagem será lida no formato que está salva (RGB ou grayscale).
         Se o nome passado for simplesmente "TEST", o programa não irá abrir um arquivo de imagem,
         mas irá usar uma funcionalidade do SciPy para criar uma imagem 512x512 em grayscale (aquela
         clássica imagem da Lena usada em processamento de imagens) para ser usada no processamento.
         A imagem em si não será salva em disco, mas a imagem resultante do processamento será salva
         seguindo o funcionamento normal do programa (isto é, será salva como "TEST-compressed.bmp").

             
    
No final da execução, o programa irá salvar a imagem processada em um arquivo com extensão .bmp
na pasta onde o EP está sendo executado, com o nome do arquivo original mais o sufixo "-compressed".
    
Exemplo, o seguinte irá processar a imagem 'fotos/casa.png', com rank 32, salvando o resultado como 'casa-compressed.bmp':
    ./EP3.py 32 fotos/casa.png
    
      
-----------------------------
ESTRUTURA DO PROGRAMA:
-----------------------------
O EP está dividido em 2 scripts python que eu escrevi. São eles:
EP3.py, que é o script principal, contendo o código para execução do programa e load/save das imagens.
Compressor.py, que contém a implementação do compressor usando SVD e a decomposição SVD em si.



=================================
PROBLEMAS COM O EP
=================================
Devido a vários outros trabalhos que tive que entregar neste final do semestre, não tive muito tempo para 
fazer esse EP.

Consequentemente, o EP não está completo. 

Se executar ele como mostrado aqui, ele irá fazer a decomposição SVD, comprimir e salvar a imagem direito,
porém somente porque ao ser executado, ele usa uma função do NumPy para fazer a decomposição SVD.

Eu tentei por muito tempo implementar a decomposição SVD sozinho, seguindo o falado no enunciado (e nos
links lá contidos) e de pesquisas na internet, mas não consegui fazer a decomposição funcionar e como 
não temos mais tempo, resolvi entregar o EP assim.

Porém, o código que fiz para realizar a decomposição SVD (mesmo não funcionando completamente) existe, e está
no arquivo Compresor.py

O primeiro passo da decomposição, o algoritmo GolubKahan, eu consegui implementar e segundo meus 
testes ele funciona (para matrizes quadradas): a matriz é bidiagonalizada corretamente. Matrizes retangulares
também são bidiagonalizadas, porém testando nota-se que nesses casos A != U*B*Vtransposto

O segundo passo (GolubReinsch) no entanto foi o que não consegui fazer funcionar. Em alguns testes dava erro, 
em outros o processo não convergia, entrando em loop infinito. Porém eu perdi muito tempo tentando fazer o 
GolubKahan funcionar, então tive menos tempo ainda para tentar mexer neste algoritmo.


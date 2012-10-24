##################################################
  MAC300 - Métodos Numéricos da Algebra Linear
  EP2 - Métodos de Processamento de Imagens
  
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
-matplotlib: biblioteca para criação de gráficos e visualização de dados.
             Instalação dessa biblioteca é opcional, mas sem ela os gráficos de
             comparação das imagens não poderão ser mostrados.
             

O programa foi testado na rede-linux com sucesso - NumPy, SciPy e matplotlib estão instalados lá.


-----------------------------
EXECUTANDO O PROGRAMA:
-----------------------------
Para executar o programa, basta rodar no terminal:
    ./EP2.py -<metodo> <nome_do_arquivo> [-compare]
    
Onde: 
    -metodo: é o nome do método de processamento (filtro) a ser aplicado na imagem.
             Pode ser qualquer um que exista no script Filters.py, e ao ser passado errado
             o programa imprime o nome dos métodos existentes. Por padrão estão implementados
             contrast, blur e sharpen.
         NOTE que o método é passado com um '-' na frente, por exemplo: ./EP2 -contrast

    nome_do_arquivo: é o nome do arquivo de imagem a ser processado. Vários formatos incluindo
                      jpg e png são aceitos, e a imagem será lida em formato grayscale.
         Se o nome passado for simplesmente "TEST", o programa não irá abrir um arquivo de imagem,
         mas irá usar uma funcionalidade do SciPy para criar uma imagem 512x512 em grayscale (aquela
         clássica imagem da Lena usada em processamento de imagens) para ser usada no processamento.
         A imagem em si não será salva em disco, mas a imagem resultante do processamento será salva
         seguindo o funcionamento normal do programa (isto é, será salva como "TEST-final.jpg").

    -compare: argumento opcional. Se passado, o programa irá mostrar ao final da execução uma
              comparação das imagens (original/processada), e outros dados relativos ao método
              de processamento usado.
             
    
No final da execução, caso a opção '-compare' foi passada e a biblioteca matplotlib está instalada,
ele irá abrir uma janela à parte para mostrar a comparação das imagens (esta janela não é aberta em
segundo plano - ela precisa ser fechada para terminar a execução).
Após isso, o programa irá salvar a imagem processada em um arquivo com extensão .jpg
na pasta onde o EP está sendo executado, com o nome do arquivo original mais o sufixo "-final".
    
Exemplo, o seguinte irá processar a imagem 'fotos/casa.png', usando o método de constraste, mostrando
a comparação, salvando o resultado como 'casa-final.jpg':
    ./EP2.py -contrast fotos/casa.png -compare
    
      
-----------------------------
ESTRUTURA DO PROGRAMA:
-----------------------------
O EP está dividido em 2 scripts python que eu escrevi. São eles:
EP2.py, que é o script principal, contendo o código para execução do programa e load/save das imagens.
Filters.py, que contém a implementação dos filtros para processamento de imagens. O script também
contém uma interface simples para definição de novos filtros que o EP2.py pode usar automaticamente.

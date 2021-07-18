# device_driver_test  
Implementação de um pseudo character device driver.  
Funcionalidade:  
Nosso driver codifica e decodifica strings utilizando a cifra de césar.  
Utilizando a função ioctl, você pode mudar o modo de operação do driver.  
São esses:  
0 - Codificar.  
1 - Decodificar.  
2 - Inserir chave.  
Em conjunto com esses modos, você deve utilizar as funções read e write.  
A função write é utilizada para mandar informações para o driver.  
Quando utilizada nos modos 0 e 1, a função write manda a string a ser processada para o driver.  
No modo 2, a função manda a chave.  
É necessário que a chave seja enviada primeiro.  
Por fim, utilizando a função read nos modos 0 ou 1, é possível obter a string processada.  
Para selecionar um modo, a função ioctl deve ser utilizada dessa forma:  
ioctl(file_descriptor, _IO(major,modo));  
Onde file_descriptor representa o int obtido ao abrir o arquivo respectivo ao driver na pasta /dev.  
major é o major number do arquivo e modo é um dos modos descritos anteriormente (0, 1 ou 2).  
Após mudar o modo de operação, você pode utilizar a função write da seguinte forma:  
write(file_descriptor, &data, data_size);  
Onde &data é o endereço dos dados a serem enviados e data_size é o seu tamanho em bytes.  
Por fim, a função read é utilizada dessa forma:  
read(file_descriptor, &data, data_size);  
E o valor de retorno será escrito no endereço fornecido em &data;  

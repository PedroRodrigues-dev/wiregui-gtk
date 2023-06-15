# Interface para wireguard feita em GTK

## Como installar via .deb

1. Baixe o arquivo .deb disponível na release mais recente.
2. Instale via terminal ou interface gráfica.
3. Execute a aplicação por meio do icone em sua interface.

# Executar via binario

1. Baixe o arquivo binario disponível na release mais recente.
2. Execute "./wiregui-gtk".

## Como compilar

1. Instale o gtkmm "sudo apt install libgtkmm-3.0-dev".
2. Compile "Make".

### Outras opções

1. make clean

apaga diretorios obj, bin, package/usr/bin e os arquivos .deb.

2. make run

compila e executa aplicação.

3. make deb

compila e cria o .deb para distribuição

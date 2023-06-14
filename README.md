# Interface para wireguard feita em GTK

## Como installar

1. Baixe o arquivo .tar disponível na release mais recente.
2. Extraia em algum lugar.
3. Use "chmod +x installer.sh"
4. Execute "./installer.sh".
5. Feche e abra novamente o terminal.
6. Inicie "wiregui-gtk".

## Como compilar

1. Instale o gtkmm "sudo apt install libgtkmm-3.0-dev".
2. Compile "Make".

### Outras opções

1. make clean

apaga diretorios obj, bin e o arquivo wiregui-gtk.tar.

2. make run

compila e executa aplicação.

3. make dist

compila e cria o wiregui-gtk.tar para distribuição

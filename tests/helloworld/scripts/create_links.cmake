execute_process(COMMAND echo "Creating symlinks for fala_ft_hello..")
execute_process(COMMAND ln -v -s /usr/bin/fala_ft_hello $ENV{DESTDIR}/usr/bin/fala_ft_hello_1)
execute_process(COMMAND ln -v -s /usr/bin/fala_ft_hello $ENV{DESTDIR}/usr/bin/fala_ft_hello_2)
execute_process(COMMAND ln -v -s /usr/bin/fala_ft_hello.launch $ENV{DESTDIR}/usr/bin/fala_ft_hello_1.launch)
execute_process(COMMAND ln -v -s /usr/bin/fala_ft_hello.launch $ENV{DESTDIR}/usr/bin/fala_ft_hello_2.launch)


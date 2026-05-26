;────────────────────── данные о разбираемом PE-файле
 header_temp_buffer    db exehdr_size+ph_size dup (?)
 ph_loc                dd ?                     ; смещение заголовка PE в файле
# Possumvibes Userspace Keymaps

This is a convenience directory for me to quickly access my in-tree keymaps. I passionately dislike navigating trees like `./keymaps/ferris/sweep/keymaps/possumvibes/`! It's awful! Symlinks are here to save the day!

I can simply cd into `users/possumvibes` and edit `keymaps/<KEYMAP-OF-MY-CHOICE>`. When I want to compile or flash, I can send the normal `qmk {compile,flash} -kb <KEYBOARD>` and have done! Since my default keymap name is set in my qmk setup, I don't have to do anything else. Keeps the boards in-tree for ease of compilation, and keeps me *out* of tree when I need to edit them! Win/win. (Technically any of the json keymaps can also be compiled in-place but that's neither here nor there)

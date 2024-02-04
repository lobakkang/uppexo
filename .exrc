let s:cpo_save=&cpo
set cpo&vim
cnoremap <silent> <Plug>(TelescopeFuzzyCommandSearch) e "lua require('telescope.builtin').command_history { default_text = [=[" . escape(getcmdline(), '"') . "]=] }"
inoremap <C-B> ^i
inoremap <C-K> <Up>
inoremap <C-H> <Left>
inoremap <C-J> <Down>
inoremap <C-L> <Right>
inoremap <C-W> u
inoremap <C-U> u
nnoremap  <Cmd> %y+ 
nnoremap  h
nnoremap <NL> j
nnoremap  k
nnoremap  l
nnoremap  <Cmd> NvimTreeToggle 
nnoremap  <Cmd> w 
tnoremap  
nnoremap  <Cmd> noh 
nnoremap  e <Cmd> NvimTreeFocus 
vnoremap  / <Cmd>lua require('Comment.api').toggle.linewise(vim.fn.visualmode())
nnoremap  gt <Cmd> Telescope git_status 
nnoremap  fw <Cmd> Telescope live_grep 
nnoremap  cm <Cmd> Telescope git_commits 
nnoremap  fa <Cmd> Telescope find_files follow=true no_ignore=true hidden=true 
nnoremap  fz <Cmd> Telescope current_buffer_fuzzy_find 
nnoremap  ma <Cmd> Telescope marks 
nnoremap  ff <Cmd> Telescope find_files 
nnoremap  fo <Cmd> Telescope oldfiles 
nnoremap  th <Cmd> Telescope themes 
nnoremap  fh <Cmd> Telescope help_tags 
nnoremap  pt <Cmd> Telescope terms 
nnoremap  fb <Cmd> Telescope buffers 
nnoremap  ch <Cmd> NvCheatsheet 
nnoremap  b <Cmd> enew 
nnoremap  fS <Cmd> LspStop 
nnoremap  n <Cmd> set nu! 
nnoremap  rn <Cmd> set rnu! 
nnoremap  fs <Cmd> LspStart 
xnoremap # y?\V"
nnoremap & :&&
xnoremap * y/\V"
nnoremap <nowait> ; :
vnoremap < <gv
vnoremap > >gv
nnoremap Y y$
xnoremap gb <Plug>(comment_toggle_blockwise_visual)
xnoremap gc <Plug>(comment_toggle_linewise_visual)
nnoremap gb <Plug>(comment_toggle_blockwise)
nnoremap gc <Plug>(comment_toggle_linewise)
xnoremap <expr> j v:count || mode(1)[0:1] == "no" ? "j" : "gj"
nnoremap <expr> j v:count || mode(1)[0:1] == "no" ? "j" : "gj"
xnoremap <expr> k v:count || mode(1)[0:1] == "no" ? "k" : "gk"
nnoremap <expr> k v:count || mode(1)[0:1] == "no" ? "k" : "gk"
xnoremap <silent> p p:let @+=@0:let @"=@0
xnoremap <Plug>(comment_toggle_blockwise_visual) <Cmd>lua require("Comment.api").locked("toggle.blockwise")(vim.fn.visualmode())
xnoremap <Plug>(comment_toggle_linewise_visual) <Cmd>lua require("Comment.api").locked("toggle.linewise")(vim.fn.visualmode())
nnoremap <Plug>PlenaryTestFile :lua require('plenary.test_harness').test_file(vim.fn.expand("%:p"))
nnoremap <C-N> <Cmd> NvimTreeToggle 
tnoremap <C-X> 
nnoremap <expr> <Up> v:count || mode(1)[0:1] == "no" ? "k" : "gk"
nnoremap <C-K> k
nnoremap <expr> <Down> v:count || mode(1)[0:1] == "no" ? "j" : "gj"
nnoremap <C-J> j
nnoremap <C-H> h
nnoremap <C-C> <Cmd> %y+ 
nnoremap <C-S> <Cmd> w 
vnoremap <expr> <Up> v:count || mode(1)[0:1] == "no" ? "k" : "gk"
vnoremap <expr> <Down> v:count || mode(1)[0:1] == "no" ? "j" : "gj"
nnoremap <C-L> l
inoremap  ^i
inoremap  <Left>
inoremap <NL> <Down>
inoremap  <Up>
inoremap  <Right>
inoremap  u
inoremap  u
let &cpo=s:cpo_save
unlet s:cpo_save
set clipboard=unnamedplus
set expandtab
set fillchars=eob:\ 
set helplang=en
set ignorecase
set laststatus=3
set noloadplugins
set mouse=a
set packpath=/usr/share/nvim/runtime
set noruler
set runtimepath=~/.config/nvim,~/.local/share/nvim/lazy/lazy.nvim,~/.local/share/nvim/lazy/Comment.nvim,~/.local/share/nvim/lazy/telescope-fzf-native.nvim,~/.local/share/nvim/lazy/telescope.nvim,~/.local/share/nvim/lazy/cmp-path,~/.local/share/nvim/lazy/cmp-buffer,~/.local/share/nvim/lazy/cmp-nvim-lsp,~/.local/share/nvim/lazy/cmp-nvim-lua,~/.local/share/nvim/lazy/cmp_luasnip,~/.local/share/nvim/lazy/nvim-autopairs,~/.local/share/nvim/lazy/friendly-snippets,~/.local/share/nvim/lazy/LuaSnip,~/.local/share/nvim/lazy/nvim-cmp,~/.local/share/nvim/lazy/better-escape.nvim,~/.local/share/nvim/lazy/plenary.nvim,~/.local/share/nvim/lazy/nvim-colorizer.lua,~/.local/share/nvim/lazy/indent-blankline.nvim,~/.local/share/nvim/lazy/null-ls.nvim,~/.local/share/nvim/lazy/nvim-lspconfig,~/.local/share/nvim/lazy/gitsigns.nvim,~/.local/share/nvim/lazy/nvim-web-devicons,~/.local/share/nvim/lazy/nvim-treesitter,~/.local/share/nvim/lazy/ui,/usr/share/nvim/runtime,/usr/lib/nvim,~/.local/state/nvim/lazy/readme,~/.local/share/nvim/lazy/cmp-path/after,~/.local/share/nvim/lazy/cmp-buffer/after,~/.local/share/nvim/lazy/cmp-nvim-lsp/after,~/.local/share/nvim/lazy/cmp-nvim-lua/after,~/.local/share/nvim/lazy/cmp_luasnip/after
set shiftwidth=2
set shortmess=lnFiIxTOotsf
set noshowmode
set showtabline=2
set smartcase
set smartindent
set softtabstop=2
set splitbelow
set splitright
set statusline=%!v:lua.require('nvchad.statusline.default').run()
set tabline=%!v:lua.require('nvchad.tabufline.modules').run()
set tabstop=2
set termguicolors
set timeoutlen=400
set undofile
set updatetime=250
set whichwrap=<>[]hl,b,s
set window=66
" vim: set ft=vim :

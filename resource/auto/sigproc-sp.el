(TeX-add-style-hook "sigproc-sp"
 (lambda ()
    (LaTeX-add-bibliographies
     "sigproc")
    (TeX-run-style-hooks
     "latex2e"
     "acm_proc_article-sp10"
     "acm_proc_article-sp"
     "")))


cmd_/home/faii/Downloads/fileaudit/kernel_module/Module.symvers := sed 's/ko$$/o/' /home/faii/Downloads/fileaudit/kernel_module/modules.order | scripts/mod/modpost -m -a  -o /home/faii/Downloads/fileaudit/kernel_module/Module.symvers -e -i Module.symvers   -T -
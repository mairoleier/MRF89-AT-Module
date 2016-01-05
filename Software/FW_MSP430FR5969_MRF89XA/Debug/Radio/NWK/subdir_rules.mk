################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Radio/NWK/aes.obj: ../Radio/NWK/aes.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmspx --abi=eabi --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="I:/Dropbox/Projects/CCS/EmbeddedSystems/MSP430FR5969_MRF89XA8_v0.1/driverlib/MSP430FR5xx_6xx" --advice:power=all --advice:hw_config=all -g --define=__MSP430FR5969__ --display_error_number --diag_wrap=off --diag_warning=225 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Radio/NWK/aes.pp" --obj_directory="Radio/NWK" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/NWK/nwk_radio.obj: ../Radio/NWK/nwk_radio.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmspx --abi=eabi --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="I:/Dropbox/Projects/CCS/EmbeddedSystems/MSP430FR5969_MRF89XA8_v0.1/driverlib/MSP430FR5xx_6xx" --advice:power=all --advice:hw_config=all -g --define=__MSP430FR5969__ --display_error_number --diag_wrap=off --diag_warning=225 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Radio/NWK/nwk_radio.pp" --obj_directory="Radio/NWK" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/NWK/nwk_security.obj: ../Radio/NWK/nwk_security.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmspx --abi=eabi --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="I:/Dropbox/Projects/CCS/EmbeddedSystems/MSP430FR5969_MRF89XA8_v0.1/driverlib/MSP430FR5xx_6xx" --advice:power=all --advice:hw_config=all -g --define=__MSP430FR5969__ --display_error_number --diag_wrap=off --diag_warning=225 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Radio/NWK/nwk_security.pp" --obj_directory="Radio/NWK" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '



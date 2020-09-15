									
#parametros									
									
param	periodo		;						
param	maxt		;						
param	cale_agua	;						
param	cale_aire	;						
param	masa_aire	;						
param	den_agua	;						
param	rags		;						
param	rsai		;						
param	raie		;						
param	ragp		;						
param	rps   		;						
param	tiag		;						
param	tiai		;						
param	tits		;						
param	titp		;						
param	vi   		;						
param	ceav		;						
param	qc   		;						
param	ce   		;						
param	ca   		;						
param	eba   		;						
param	ebt    		;						
param	ect   		;						
param	taimin		;						
param	taimax		;						
param	tpmin		;						
param	tpmax		;						
param	tamax		;	
param   tamin		;
param 	aba			;
param	Hrel		;
param	Hmin		;
param	Hmax		;
param	Hin			;



param mucho :=	100;				
set	TIME	:=							
0	..	maxt							
;									
param 	te	{TIME}>0	;						
									
					;				
#	vars				;				
#temperatura					;				
	var TAI		{t in TIME}		;				
						   			
									
	var TAG		{t in TIME}		;				
									
									
									
						   			
									
									
#decision					;				
					
	var BT		{t in TIME}	binary	;								#activacion de la bomba de calefaccion			
					
	var VA		{t in TIME}	>=0		;								#volumen de agua en litros
	
	var BA		{t in TIME}	binary	;								#activacion de la bomba de irrigacion
	
	var costo_feo 		>=0			;								#costo de holgura para los limites de temperatura
	
	var HUM		{t in TIME}	>=0		;								#humedad relativa del suelo en porcentaje [0-100]
									
#FO									
	minimize FO: (( sum {t in TIME} ce*( BT[t]*(ebt + ect)))+mucho*costo_feo+ (sum{t in TIME} BA[t]*aba*ca ));		
									
#S.A.									
#1									
	subject to lim_inf_temp_ai 	{t in 1..maxt}	:					#condiciones limite del aire para la planta		
		taimin	-costo_feo<=	TAI[t]	;				
									
#2									
	subject to lim_sup_temp_ai 	{t in 1..maxt}	:					#condiciones limite del aire para la planta		
		TAI[t]	<=	taimax	+costo_feo;				
									
#3									
	subject to lim_sup_temp_ag 	{t in 1..maxt}	:					#condiciones limite del aire para el calefactor		
		TAG[t]	<=	tamax	+costo_feo;	
		
#4								
	subject to lim_inf_temp_ag 	{t in 1..maxt}	:
		tamin  -costo_feo    <=  TAG[t]	;
		
#5							
	subject to temp_agua       	{t in 1..maxt}	:					#nueva temperatura del agua		
		TAG[t]	=	TAG[t-1]+	((BT[t-1]*qc*1000 + 8*(TAI[t-1]-TAG[t-1])/(rags+rsai) )*periodo/(VA[t]*cale_agua*den_agua)) ; #	aqui es donde tengo la complicacion, el 4 deve remplazarce por BA[t], ahora esta considerenado un volumen constnate de agua de 4 litros para solo este calculo
#6
	subject to temp_aire		{t in 1..maxt}	:					#nueva temperatura del aire
		TAI[t]	= 	TAI[t-1]+	( -8*(TAI[t-1]-TAG[t-1])/(rags+rsai) + (te[t-1]-TAI[t-1])/(raie) ) *periodo/(masa_aire*cale_aire);
		
#7		
	subject to temp_inicial_agua 				:					#temperatura inicial del aire
		TAG[0]	= 	tiag;
		
#8									
	subject to temp_inicial_aire 				:					#temperatura inicial del aire
		TAI[0]	= 	tiai;
		
#9,1									
	subject to consumo_agua 	{t in 1..maxt}	:					# consumo del agua en el tiempo con y sin irrigacion en el tiempo t
		HUM[t]	<= 	(HUM[t-1] + (-10)*(1-BA[t]))
					+(BA[t])*(Hrel)
					;
#9,2
	subject to consumo_agua2	{t in 1..maxt}	:					#relacion consumo de agua por el uso de la bomba
		VA[t]	=	VA[t-1] - (aba)*(BA[t-1]);

#10
	subject to hum_minima		{t in 1..maxt}	:					#limite inferior para la humedad
		Hmin - costo_feo	<=	HUM[t];
	
#11
	subject to hum_maxima		{t in 1..maxt}	:					#limite superior para la humedad
		HUM[t]	<=	Hmax + costo_feo;

#12
	subject to hum_en_pociento	{t in TIME}		:					#limite absoluto de 100% para la humedad
		HUM[t]	<= 100;
		
#13
	subject to hum_inicial						:					#humedad inicial	
		HUM[0]	=	Hin;
		
#14
	subject to agua_inicial						:					#volumnet inicial del agua
		VA[0]	=	vi;
		
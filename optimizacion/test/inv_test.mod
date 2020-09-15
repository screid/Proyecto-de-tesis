									
								
#sets								
param	periodo		;						
param	maxt		;
set	TIME	:=	0 .. maxt by periodo	;				#tiempos en segundos sepoarados por periodo segundos
set VARS	=	{"cfais","cfaii","cfags","cfagi","cfhrs","cfhri"};	#holguras para cada limite
set TIEMPOS :=  periodo .. maxt by periodo;

#parametros						
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
param 	mucho {var in VARS};				

								
param 	te	{TIME}>0	;

									
#	vars
#temperatura
	var TAI		{t in TIME}			;				
						   			
									
	var TAG		{t in TIME}			;				
									
									
	var HUM		{t in TIME}	>=0		;								#humedad relativa del suelo en porcentaje [0-100]
								
						   			
									
									
#decision					;				
					
	var BT		{t in TIME}	binary	;								#activacion de la bomba de calefaccion			
					
	var VA		{t in TIME}	>=0		;								#volumen de agua en litros
	
	var BA		{t in TIME}	binary	;								#activacion de la bomba de irrigacion
	
	var CF		{t in TIME , var in VARS} 		>=0			;		#costo feos de holgura para los limites de temperatura
	
										
#FO									
	minimize FO: (  sum {t in TIEMPOS}( ce*( BT[t]*(ebt + ect)) + BA[t]*aba*ca )+ sum {t in TIEMPOS , var in VARS} mucho[var]*CF[t,var] );		
									
#S.A.									
#1									
	subject to lim_inf_temp_ai 	{t in TIEMPOS}	:					#condiciones limite del aire para la planta		
		taimin	<=	TAI[t]	+CF[t,"cfaii"];				
									
#2									
	subject to lim_sup_temp_ai 	{t in TIEMPOS}	:					#condiciones limite del aire para la planta		
		TAI[t]	<=	taimax	+CF[t,"cfais"];				
									
#3									
	subject to lim_sup_temp_ag 	{t in TIEMPOS}	:					#condiciones limite del aire para el calefactor		
		TAG[t]	<=	tamax	+CF[t,"cfags"];	
		
#4								
	subject to lim_inf_temp_ag 	{t in TIEMPOS}	:
		tamin   <=  TAG[t]	+CF[t,"cfagi"];
		
#5							
	subject to temp_agua       	{t in TIEMPOS}	:					#nueva temperatura del agua		
		TAG[t]	=	TAG[t-periodo]+	((BT[t-periodo]*qc*1000 + 8*(TAI[t-periodo]-TAG[t-periodo])/(rags+rsai) )*periodo/(VA[t]*cale_agua*den_agua)) ; #	aqui es donde tengo la complicacion, el 4 deve remplazarce por BA[t], ahora esta considerenado un volumen constnate de agua de 4 litros para solo este calculo
#6
	subject to temp_aire		{t in TIEMPOS}	:					#nueva temperatura del aire
		TAI[t]	= 	TAI[t-periodo]+	( -8*(TAI[t-periodo]-TAG[t-periodo])/(rags+rsai) + (te[t-periodo]-TAI[t-periodo])/(raie) ) *periodo/(masa_aire*cale_aire);
		
#7		
	subject to temp_inicial_agua 				:					#temperatura inicial del aire
		TAG[0]	= 	tiag;
		
#8									
	subject to temp_inicial_aire 				:					#temperatura inicial del aire
		TAI[0]	= 	tiai;
		
#9,1									
	subject to consumo_agua 	{t in TIEMPOS}	:					# consumo del agua en el tiempo con y sin irrigacion en el tiempo t
		HUM[t]	<= 	(HUM[t-periodo] + (-10)*(1-BA[t]))
					+(BA[t])*(Hrel)
					;
#9,2
	subject to consumo_agua2	{t in TIEMPOS}	:					#relacion consumo de agua por el uso de la bomba
		VA[t]	=	VA[t-periodo] - (aba)*(BA[t-periodo]);

#10
	subject to hum_minima		{t in TIEMPOS}	:					#limite inferior para la humedad
		Hmin	<=	HUM[t] +CF[t,"cfhri"];
	
#11
	subject to hum_maxima		{t in TIEMPOS}	:					#limite superior para la humedad
		HUM[t]	<=	Hmax + CF[t,"cfhrs"];

#12
	subject to hum_en_pociento	{t in TIEMPOS}		:					#limite absoluto de 100% para la humedad
		HUM[t]	<= 100;
		
#13
	subject to hum_inicial						:					#humedad inicial	
		HUM[0]	=	Hin;
		
#14
	subject to agua_inicial						:					#volumnet inicial del agua
		VA[0]	=	vi;
		
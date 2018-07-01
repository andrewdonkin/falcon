// Zeger Knaepen
// http://news.povray.org/povray.binaries.images/thread/%3C4470cc30%241%40news.povray.org%3E/
#default {pigment {rgb 1} finish {ambient .1 specular 1}}
camera {
	location <2.8,-1,-3>
	angle 18
	look_at 0
}
#declare Seed=seed(441);
light_source {<-750,500,-500> rgb 1} //<1.5,1.4,1.3>}
light_source {<-750,-500,-500> rgb <.5,.4,.3>}
light_source {<500,150,750> rgb <.5,.6,.7>}


//#include "macros.inc"
#macro RoundCilinderSegment(StartHeight,EindHeight,Radius,StartHoek,EindHoek,Bevel,BevelHeight,MaxLength)
	#local Start=y*(StartHeight+Bevel);
	#local Eind=y*(EindHeight-Bevel);
	#local Start2=y*StartHeight;
	#local Eind2=y*EindHeight;
		#local StartHoek2=StartHoek+degrees(Bevel/Radius);
		#local EindHoek2=EindHoek-degrees(Bevel/Radius);
		#local TotaleLengte=radians(EindHoek-StartHoek)*(Radius+BevelHeight);
		#local Aantal=max(int(TotaleLengte/MaxLength),1);
		#local Tel=0;
		mesh {
			triangle {
				vrotate(<Radius,StartHeight,0>,y*StartHoek),
				vrotate(<Radius,EindHeight,0>,y*StartHoek),
				vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*StartHoek2)
			}
			triangle {
				vrotate(<Radius,EindHeight,0>,y*StartHoek),
				vrotate(<Radius+BevelHeight,EindHeight-Bevel,0>,y*StartHoek2),
				vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*StartHoek2)
			}
			triangle {
				vrotate(<Radius,EindHeight,0>,y*StartHoek),
				vrotate(<Radius,EindHeight,0>,y*StartHoek2),
				vrotate(<Radius+BevelHeight,EindHeight-Bevel,0>,y*StartHoek2)
			}
			triangle {
				vrotate(<Radius,StartHeight,0>,y*StartHoek),
				vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*StartHoek2),
				vrotate(<Radius,StartHeight,0>,y*StartHoek2)
			}

			triangle {
				vrotate(<Radius,StartHeight,0>,y*EindHoek),
				vrotate(<Radius,EindHeight,0>,y*EindHoek),
				vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*EindHoek2)
			}
			triangle {
				vrotate(<Radius,EindHeight,0>,y*EindHoek),
				vrotate(<Radius+BevelHeight,EindHeight-Bevel,0>,y*EindHoek2),
				vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*EindHoek2)
			}
			triangle {
				vrotate(<Radius,EindHeight,0>,y*EindHoek),
				vrotate(<Radius,EindHeight,0>,y*EindHoek2),
				vrotate(<Radius+BevelHeight,EindHeight-Bevel,0>,y*EindHoek2)
			}
			triangle {
				vrotate(<Radius,StartHeight,0>,y*EindHoek),
				vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*EindHoek2),
				vrotate(<Radius,StartHeight,0>,y*EindHoek2)
			}

			#while (Tel<Aantal)
				#local H1=StartHoek2+(EindHoek2-StartHoek2)*(Tel/Aantal);
				#local H2=StartHoek2+(EindHoek2-StartHoek2)*((Tel+1)/Aantal);
				#local P1=vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*H1);
				#local P2=vrotate(<Radius+BevelHeight,EindHeight-Bevel,0>,y*H1);
				#local P3=vrotate(<Radius+BevelHeight,EindHeight-Bevel,0>,y*H2);
				#local P4=vrotate(<Radius+BevelHeight,StartHeight+Bevel,0>,y*H2);
				
				#local N1=vrotate(x,y*H1);
				#local N2=vrotate(x,y*H2);
				smooth_triangle {P1,N1,P2,N1,P4,N2}
				smooth_triangle {P2,N1,P3,N2,P4,N2}
				//triangle {P1,P2,P4}
				//triangle {P2,P3,P4}
				
				#local P2B=vrotate(<Radius,EindHeight,0>,y*H1);
				#local P3B=vrotate(<Radius,EindHeight,0>,y*H2);
				triangle {P2,P2B,P3}
				triangle {P2B,P3B,P3}

				#local P2B=vrotate(<Radius,StartHeight,0>,y*H2);
				#local P3B=vrotate(<Radius,StartHeight,0>,y*H1);
				triangle {P2B,P1,P3B}
				triangle {P1,P4,P2B}
				#local Tel=Tel+1;
			#end
		}
#end
#macro Verlaag(X) // reduce
	#local R=X-(1+rand(Seed)*2.5);
	R
#end
#macro CilinderGreeble(StartHeight,EindHeight,Radius,StartHoek,EindHoek,Bevel,BevelHeight,MaxLength,Detail)
	#local Start=(StartHeight+Bevel);
	#local Eind=(EindHeight-Bevel);
	#local StartH=StartHoek+degrees(Bevel/Radius);
	#local EindH=EindHoek-degrees(Bevel/Radius);
	//cilindertjes:  cylinders:
	#local Aantal=rand(Seed)*Detail;
	#while (Aantal>0)
		#local Size=(Bevel*.01)+rand(Seed)*Bevel*.99*2;
		#local Hoek=StartH+(EindH-StartH)*rand(Seed);
		#local Hoogte=Start+(Eind-Start)*rand(Seed);
		#local Depth=BevelHeight/2+rand(Seed)*BevelHeight/2;
		superellipsoid {<1,.1+rand(Seed)*.3> rotate y*90  scale <Depth,Size,Size> translate x*Radius rotate y*Hoek translate y*Hoogte}
		#local Aantal=Aantal-1;
	#end
	//vakjes:  boxes
	#local Aantal=rand(Seed)*Detail;
	#while (Aantal>0)
		#local S=Start+(Eind-Start)*rand(Seed);
		#local E=S+(Eind-S)*rand(Seed);
		#local SH=StartH+(EindH-StartH)*rand(Seed);
		#local EH=SH+(EindH-SH)*rand(Seed);
		#local H=BevelHeight/2+rand(Seed)*BevelHeight/2;
//		RoundCilinderSegment(S,E,Radius,SH,EH,Bevel/2,H,AantalStapjes)
		#local Aantal=Aantal-1;
	#end
	//verticale buisjes, tubes:
	#local Aantal=rand(Seed)*Detail;
	#while (Aantal>0)
		#local S=Start+(Eind-Start)*rand(Seed);
		#local E=S+(Eind-S)*rand(Seed);
		#local SH=StartH+(EindH-StartH)*rand(Seed);
		//#local EH=SH+(EindH-SH)*rand(Seed);
		#local H=(BevelHeight/2+rand(Seed)*BevelHeight/2)/2;
		//RoundCilinderSegment(S,E,Radius,SH,EH,Bevel/2,H,AantalStapjes)
//		cylinder {y*S,y*E,H translate x*Radius rotate y*SH}
//		sphere {y*S,H translate x*Radius rotate y*SH}
//		sphere {y*E,H translate x*Radius rotate y*SH}
		#local Aantal=Aantal-1;
	#end
	//horizontale buisjes simuleren, toruskes zouden wsl te traag gaan
	// simulated horizontel tubes, torii would be slow
	#local Aantal=rand(Seed)*Detail;   // quantity
	#while (Aantal>0)
		#local S=Start+(Eind-Start)*rand(Seed);
		#local H=BevelHeight/2+rand(Seed)*BevelHeight/2;
		#local H=min((Eind-S),H*2)/2;
		#local E=S+H*2;
		#local SH=StartH+(EindH-StartH)*rand(Seed);
		#local EH=SH+(EindH-SH)*rand(Seed);
//		RoundCilinderSegment(S,E,Radius,SH,EH,H,H,AantalStapjes)
		#local Aantal=Aantal-1;
	#end
#end
#macro VerdeelCilinder(StartHeight,EindHeight,Radius,StartHoek,EindHoek,Diepte,Bevel,BevelHeight)
	#local Start=y*(StartHeight+Bevel);
	#local Eind=y*(EindHeight-Bevel);
	#local Start2=y*StartHeight;
	#local Eind2=y*EindHeight;
	#if (Diepte<0) // depth
		#ifndef(Detail) #local Detail=5; #end
		#local H=rand(Seed)*BevelHeight;
		#ifndef(MaxSegmentLength) #local AantalStapjes=Bevel; //(eigenlijk de maximale lengte van een segment)
		// previous is actually maximum length of a segment
		#else #local AantalStapjes= MaxSegmentLength; #end
		RoundCilinderSegment(StartHeight,EindHeight,Radius,StartHoek,EindHoek,Bevel,H,AantalStapjes)
		CilinderGreeble(StartHeight,EindHeight,Radius+H,StartHoek,EindHoek,Bevel,H,AantalStapjes,Detail)
	#else
		// eerst es checken of 't nie onvoorstelbaar ongelijk verdeeld is:
		// first check if it is not unimaginably [!? google translate] divided into:
		#local Lengte=radians(EindHoek-StartHoek)*Radius;
		#local Hoogte=EindHeight-StartHeight;
		
		#local PercentageU=.5+(rand(Seed)-rand(Seed))*.5*.9;
		#local PercentageV=.5+(rand(Seed)-rand(Seed))*.5*.9;
		#local MidHeight=StartHeight+(EindHeight-StartHeight)*PercentageU;
		#local MidHoek=StartHoek+(EindHoek-StartHoek)*PercentageV;

		// als't 2 keer zo hoog als breed is (of nog hoger), dan ff enkel in de hoogte bijsnijden:
		// if it's twice as high as wide (or even higher), then ff just cuts up:
		#if ((Hoogte>(Lengte*2))&(rand(Seed)>.125))
			VerdeelCilinder(StartHeight,MidHeight,Radius,StartHoek,EindHoek,Verlaag(Diepte),Bevel,BevelHeight)
			VerdeelCilinder(MidHeight,EindHeight,Radius,StartHoek,EindHoek,Verlaag(Diepte),Bevel,BevelHeight)
		// als't 2 keer lager dan breed is (of nog lager), dan ff enkel in de breedte bijsnijden:
		// if it's half as high as wide (or even shorter), then ff only in width
		#else 
			#if (((Hoogte*2)<Lengte)&(rand(Seed)>.125))
				VerdeelCilinder(StartHeight,EindHeight,Radius,StartHoek,MidHoek,Verlaag(Diepte),Bevel,BevelHeight)
				VerdeelCilinder(StartHeight,EindHeight,Radius,MidHoek,EindHoek,Verlaag(Diepte),Bevel,BevelHeight)
			#else
				VerdeelCilinder(StartHeight,MidHeight,Radius,StartHoek,MidHoek,Verlaag(Diepte),Bevel,BevelHeight)
				VerdeelCilinder(MidHeight,EindHeight,Radius,StartHoek,MidHoek,Verlaag(Diepte),Bevel,BevelHeight)
				
				VerdeelCilinder(StartHeight,MidHeight,Radius,MidHoek,EindHoek,Verlaag(Diepte),Bevel,BevelHeight)
				VerdeelCilinder(MidHeight,EindHeight,Radius,MidHoek,EindHoek,Verlaag(Diepte),Bevel,BevelHeight)
			#end
		#end
	#end
#end
//#include "textures.inc"
union {
	#declare Detail=5;
	#declare MaxSegmentLength=.1;
	VerdeelCilinder(-1,1,1,0,140,7,.002 * 8,.0075 * 1) 
	//cylinder {-3*y,3*y,1}
	//Metaal()
	rotate z*90
}

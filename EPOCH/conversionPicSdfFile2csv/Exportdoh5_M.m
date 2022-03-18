function Exportdoh5(cesta1,cesta_ulozeni,lambda)
addpath(genpath('/gpfs/home/mzakova/scratch/2D/vizualizace_final2/data'))

   if exist(cesta_ulozeni,'dir')==0 %kdyz neni slozka k ulozeni tak ji vytvor
   mkdir(cesta_ulozeni)
   end

c=299792458;
m_e=9.10938e-31;
e=1.60217662e-19;
eps0=8.85419e-12;
perioda=lambda / c;
frekvence=2*pi*c/lambda;
nc=frekvence^2*m_e*eps0/e^2;
hmotnost_proton=1836.2;
hmotnost_carbon=22032;
cd(cesta1)


for j=1:9
 cisloSDFef=sprintf('ef000%d.sdf',j);
        cisloSDFp=sprintf('p000%d.sdf',j);
        cisloSDFd=sprintf('d000%d.sdf',j);
       
     cd(cesta1)   
    l=GetDataSDF(cisloSDFef);
    ld=GetDataSDF(cisloSDFd); 
    lp=GetDataSDF(cisloSDFp); 
    if isstruct(l)
        x=l.Electric_Field.Ey.grid.x*1e6;   % in um
        y=l.Electric_Field.Ey.grid.y*1e6;   % in um
        cas=l.time*1e15;   % in fs
        Ey1=l.Electric_Field.Ey.data;
        Ex1=l.Electric_Field.Ex.data; 
        a0_y = e*Ey1/(m_e*c*frekvence);   % dimensionless
        a0vykr_y = imresize(a0_y, 0.2);
        a0_x = e*Ex1/(m_e*c*frekvence);   % dimensionless
	    a0vykr_x = imresize(a0_x, 0.2);
        hustel=ld.Derived.Number_Density.proton.data./nc;  
	    hustelvykr = imresize(hustel, 0.2);
        hustel_c=ld.Derived.Number_Density.carbon.data./nc;  
	    hustelvykr_c = imresize(hustel_c, 0.2);
    	xvykr=x(1:end-1);
     	xvykr = imresize(xvykr, 0.2);
	    yvykr=y(1:end-1);
	    yvykr = imresize(yvykr, 0.2);


	px_p= lp.Particles.Px.subset_ionbackground.proton.data;
	py_p= lp.Particles.Py.subset_ionbackground.proton.data;
	pz_p=lp.Particles.Pz.subset_ionbackground.proton.data;
    xx_p=lp.Particles.Px.subset_ionbackground.proton.grid.x;
    yy_p=lp.Particles.Px.subset_ionbackground.proton.grid.y;
	px_p1=px_p./(m_e*hmotnost_proton*c);
	py_p1=py_p./(m_e*hmotnost_proton*c);
	pz_p1=pz_p./(m_e*hmotnost_proton*c);
        %podm_p=find(px_p1>0.0 & px_p1=0); % berou se jen částice s hybností v kladném či nezáporném směru
        %px_p2=px_p1(podm_p);
        %py_p2=py_p1(podm_p);
        %pz_p2=pz_p1(podm_p);
        gamma_p=sqrt(1.0+px_p1.^2+py_p1.^2+pz_p1.^2);
        energy_p=0.511*hmotnost_proton*(gamma_p-1.0);



	px_c= lp.Particles.Px.subset_ionbackground.carbon.data;
	py_c= lp.Particles.Py.subset_ionbackground.carbon.data;
	pz_c=lp.Particles.Pz.subset_ionbackground.carbon.data;
    xx_c=lp.Particles.Px.subset_ionbackground.carbon.grid.x;
    yy_c=lp.Particles.Px.subset_ionbackground.carbon.grid.y;
	px_c1=px_c./(m_e*hmotnost_carbon*c);
	py_c1=py_c./(m_e*hmotnost_carbon*c);
	pz_c1=pz_c./(m_e*hmotnost_carbon*c);
        %podm_c=find(px_c1>0.0 & px_c1=0); % berou se jen částice s hybností v kladném či nezáporném směru
        %px_c2=px_c1(podm_c);
        %py_c2=py_c1(podm_c);
        %pz_c2=pz_c1(podm_c);
        gamma_c=sqrt(1.0+px_c1.^2+py_c1.^2+pz_c1.^2);
        energy_c=0.511*hmotnost_carbon*(gamma_c-1.0);

    
    [s1,s2]=size(a0vykr_y);
    [s3]=length(energy_p);
    [s4]=length(energy_c);
         
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/a0_y',[s1 s2])
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/a0_x',[s1 s2])
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/x',[s1 1])
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/y',[s2 1])
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/proton',[s1 s2])
            h5create(sprintf('elimaia_fields-000%d.h5',j),'/carbon',[s1 s2])
            h5write(sprintf('elimaia_fields-000%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_fields-000%d.h5',j), '/a0_y', a0vykr_y)
            h5write(sprintf('elimaia_fields-000%d.h5',j), '/a0_x', a0vykr_x)
            h5write(sprintf('elimaia_fields-000%d.h5',j), '/x', xvykr)
            h5write(sprintf('elimaia_fields-000%d.h5',j), '/y', yvykr)
            h5write(sprintf('elimaia_fields-000%d.h5',j), '/proton', hustelvykr)
            h5write(sprintf('elimaia_fields-000%d.h5',j), '/carbon', hustelvykr_c)
            
            h5create(sprintf('elimaia_proton-000%d.h5',j),'/energy_p',[s3 1])
            h5create(sprintf('elimaia_proton-000%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_proton-000%d.h5',j), '/x_part_p',[s3 1])
            h5create(sprintf('elimaia_proton-000%d.h5',j), '/y_part_p', [s3 1])
            h5write(sprintf('elimaia_proton-000%d.h5',j), '/x_part_p', xx_p)
            h5write(sprintf('elimaia_proton-000%d.h5',j), '/y_part_p', yy_p)
            h5write(sprintf('elimaia_proton-000%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_proton-000%d.h5',j), '/energy_p', energy_p)
            
            h5create(sprintf('elimaia_carbon-000%d.h5',j),'/energy_c',[s4 1])
            h5create(sprintf('elimaia_carbon-000%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_carbon-000%d.h5',j), '/x_part_c',[s4 1])
            h5create(sprintf('elimaia_carbon-000%d.h5',j), '/y_part_c', [s4 1])
            h5write(sprintf('elimaia_carbon-000%d.h5',j), '/x_part_c', xx_c)
            h5write(sprintf('elimaia_carbon-000%d.h5',j), '/y_part_c', yy_c)
            h5write(sprintf('elimaia_carbon-000%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_carbon-000%d.h5',j), '/energy_c', energy_c) 
            
           
  
  
          
        close
    end 
   
end



for j=10:99

        cisloSDFef=sprintf('ef00%d.sdf',j);
        cisloSDFp=sprintf('p00%d.sdf',j);
        cisloSDFd=sprintf('d00%d.sdf',j);
       
     cd(cesta1)   
    l=GetDataSDF(cisloSDFef);
    ld=GetDataSDF(cisloSDFd); 
    lp=GetDataSDF(cisloSDFp); 
    if isstruct(l)
        x=l.Electric_Field.Ey.grid.x*1e6;   % in um
        y=l.Electric_Field.Ey.grid.y*1e6;   % in um
        cas=l.time*1e15;   % in fs
        Ey1=l.Electric_Field.Ey.data;
        Ex1=l.Electric_Field.Ex.data; 
        a0_y = e*Ey1/(m_e*c*frekvence);   % dimensionless
        a0vykr_y = imresize(a0_y, 0.2);
        a0_x = e*Ex1/(m_e*c*frekvence);   % dimensionless
	    a0vykr_x = imresize(a0_x, 0.2);
        hustel=ld.Derived.Number_Density.proton.data./nc;  
	    hustelvykr = imresize(hustel, 0.2);
        hustel_c=ld.Derived.Number_Density.carbon.data./nc;  
	    hustelvykr_c = imresize(hustel_c, 0.2);
    	xvykr=x(1:end-1);
     	xvykr = imresize(xvykr, 0.2);
	    yvykr=y(1:end-1);
	    yvykr = imresize(yvykr, 0.2);


	px_p= lp.Particles.Px.subset_ionbackground.proton.data;
	py_p= lp.Particles.Py.subset_ionbackground.proton.data;
	pz_p=lp.Particles.Pz.subset_ionbackground.proton.data;
    xx_p=lp.Particles.Px.subset_ionbackground.proton.grid.x;
    yy_p=lp.Particles.Px.subset_ionbackground.proton.grid.y;
	px_p1=px_p./(m_e*hmotnost_proton*c);
	py_p1=py_p./(m_e*hmotnost_proton*c);
	pz_p1=pz_p./(m_e*hmotnost_proton*c);
        %podm_p=find(px_p1>0.0 & px_p1=0); % berou se jen částice s hybností v kladném či nezáporném směru
        %px_p2=px_p1(podm_p);
        %py_p2=py_p1(podm_p);
        %pz_p2=pz_p1(podm_p);
        gamma_p=sqrt(1.0+px_p1.^2+py_p1.^2+pz_p1.^2);
        energy_p=0.511*hmotnost_proton*(gamma_p-1.0);



	px_c= lp.Particles.Px.subset_ionbackground.carbon.data;
	py_c= lp.Particles.Py.subset_ionbackground.carbon.data;
	pz_c=lp.Particles.Pz.subset_ionbackground.carbon.data;
    xx_c=lp.Particles.Px.subset_ionbackground.carbon.grid.x;
    yy_c=lp.Particles.Px.subset_ionbackground.carbon.grid.y;
	px_c1=px_c./(m_e*hmotnost_carbon*c);
	py_c1=py_c./(m_e*hmotnost_carbon*c);
	pz_c1=pz_c./(m_e*hmotnost_carbon*c);
        %podm_c=find(px_c1>0.0 & px_c1=0); % berou se jen částice s hybností v kladném či nezáporném směru
        %px_c2=px_c1(podm_c);
        %py_c2=py_c1(podm_c);
        %pz_c2=pz_c1(podm_c);
        gamma_c=sqrt(1.0+px_c1.^2+py_c1.^2+pz_c1.^2);
        energy_c=0.511*hmotnost_carbon*(gamma_c-1.0);

    
    [s1,s2]=size(a0vykr_y);
    [s3]=length(energy_p);
    [s4]=length(energy_c);
         
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/a0_y',[s1 s2])
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/a0_x',[s1 s2])
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/x',[s1 1])
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/y',[s2 1])
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/proton',[s1 s2])
            h5create(sprintf('elimaia_fields-00%d.h5',j),'/carbon',[s1 s2])
            h5write(sprintf('elimaia_fields-00%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_fields-00%d.h5',j), '/a0_y', a0vykr_y)
            h5write(sprintf('elimaia_fields-00%d.h5',j), '/a0_x', a0vykr_x)
            h5write(sprintf('elimaia_fields-00%d.h5',j), '/x', xvykr)
            h5write(sprintf('elimaia_fields-00%d.h5',j), '/y', yvykr)
            h5write(sprintf('elimaia_fields-00%d.h5',j), '/proton', hustelvykr)
            h5write(sprintf('elimaia_fields-00%d.h5',j), '/carbon', hustelvykr_c)
            
            h5create(sprintf('elimaia_proton-00%d.h5',j),'/energy_p',[s3 1])
            h5create(sprintf('elimaia_proton-00%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_proton-00%d.h5',j), '/x_part_p',[s3 1])
            h5create(sprintf('elimaia_proton-00%d.h5',j), '/y_part_p', [s3 1])
            h5write(sprintf('elimaia_proton-00%d.h5',j), '/x_part_p', xx_p)
            h5write(sprintf('elimaia_proton-00%d.h5',j), '/y_part_p', yy_p)
            h5write(sprintf('elimaia_proton-00%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_proton-00%d.h5',j), '/energy_p', energy_p)
            
            h5create(sprintf('elimaia_carbon-00%d.h5',j),'/energy_c',[s4 1])
            h5create(sprintf('elimaia_carbon-00%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_carbon-00%d.h5',j), '/x_part_c',[s4 1])
            h5create(sprintf('elimaia_carbon-00%d.h5',j), '/y_part_c', [s4 1])
            h5write(sprintf('elimaia_carbon-00%d.h5',j), '/x_part_c', xx_c)
            h5write(sprintf('elimaia_carbon-00%d.h5',j), '/y_part_c', yy_c)
            h5write(sprintf('elimaia_carbon-00%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_carbon-00%d.h5',j), '/energy_c', energy_c) 
            
           
  
  
          
        close
    end 

   
end

for j=100:999

   cisloSDFef=sprintf('ef0%d.sdf',j);
   cisloSDFp=sprintf('p0%d.sdf',j);
   cisloSDFd=sprintf('d0%d.sdf',j);
       
        
    l=GetDataSDF(cisloSDFef);
    ld=GetDataSDF(cisloSDFd); 
    lp=GetDataSDF(cisloSDFp); 
    if isstruct(l)
        x=l.Electric_Field.Ey.grid.x*1e6;   % in um
        y=l.Electric_Field.Ey.grid.y*1e6;   % in um
        cas=l.time*1e15;   % in fs
        Ey1=l.Electric_Field.Ey.data;
        Ex1=l.Electric_Field.Ex.data; 
        a0_y = e*Ey1/(m_e*c*frekvence);   % dimensionless
        a0vykr_y = imresize(a0_y, 0.2);
        a0_x = e*Ex1/(m_e*c*frekvence);   % dimensionless
	    a0vykr_x = imresize(a0_x, 0.2);
        hustel=ld.Derived.Number_Density.proton.data./nc;  
	    hustelvykr = imresize(hustel, 0.2);
        hustel_c=ld.Derived.Number_Density.carbon.data./nc;  
	    hustelvykr_c = imresize(hustel_c, 0.2);
    	xvykr=x(1:end-1);
     	xvykr = imresize(xvykr, 0.2);
	    yvykr=y(1:end-1);
	    yvykr = imresize(yvykr, 0.2);


	px_p= lp.Particles.Px.subset_ionbackground.proton.data;
	py_p= lp.Particles.Py.subset_ionbackground.proton.data;
	pz_p=lp.Particles.Pz.subset_ionbackground.proton.data;
    xx_p=lp.Particles.Px.subset_ionbackground.proton.grid.x;
    yy_p=lp.Particles.Px.subset_ionbackground.proton.grid.y;
	px_p1=px_p./(m_e*hmotnost_proton*c);
	py_p1=py_p./(m_e*hmotnost_proton*c);
	pz_p1=pz_p./(m_e*hmotnost_proton*c);
        %podm_p=find(px_p1>0.0 & px_p1=0); % berou se jen částice s hybností v kladném či nezáporném směru
        %px_p2=px_p1(podm_p);
        %py_p2=py_p1(podm_p);
        %pz_p2=pz_p1(podm_p);
        gamma_p=sqrt(1.0+px_p1.^2+py_p1.^2+pz_p1.^2);
        energy_p=0.511*hmotnost_proton*(gamma_p-1.0);



	px_c= lp.Particles.Px.subset_ionbackground.carbon.data;
	py_c= lp.Particles.Py.subset_ionbackground.carbon.data;
	pz_c=lp.Particles.Pz.subset_ionbackground.carbon.data;
    xx_c=lp.Particles.Px.subset_ionbackground.carbon.grid.x;
    yy_c=lp.Particles.Px.subset_ionbackground.carbon.grid.y;
	px_c1=px_c./(m_e*hmotnost_carbon*c);
	py_c1=py_c./(m_e*hmotnost_carbon*c);
	pz_c1=pz_c./(m_e*hmotnost_carbon*c);
        %podm_c=find(px_c1>0.0 & px_c1=0); % berou se jen částice s hybností v kladném či nezáporném směru
        %px_c2=px_c1(podm_c);
        %py_c2=py_c1(podm_c);
        %pz_c2=pz_c1(podm_c);
        gamma_c=sqrt(1.0+px_c1.^2+py_c1.^2+pz_c1.^2);
        energy_c=0.511*hmotnost_carbon*(gamma_c-1.0);

    
    [s1,s2]=size(a0vykr_y);
    [s3]=length(energy_p);
    [s4]=length(energy_c);
         
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/a0_y',[s1 s2])
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/a0_x',[s1 s2])
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/x',[s1 1])
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/y',[s2 1])
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/proton',[s1 s2])
            h5create(sprintf('elimaia_fields-0%d.h5',j),'/carbon',[s1 s2])
            h5write(sprintf('elimaia_fields-0%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_fields-0%d.h5',j), '/a0_y', a0vykr_y)
            h5write(sprintf('elimaia_fields-0%d.h5',j), '/a0_x', a0vykr_x)
            h5write(sprintf('elimaia_fields-0%d.h5',j), '/x', xvykr)
            h5write(sprintf('elimaia_fields-0%d.h5',j), '/y', yvykr)
            h5write(sprintf('elimaia_fields-0%d.h5',j), '/proton', hustelvykr)
            h5write(sprintf('elimaia_fields-0%d.h5',j), '/carbon', hustelvykr_c)
            
            h5create(sprintf('elimaia_proton-0%d.h5',j),'/energy_p',[s3 1])
            h5create(sprintf('elimaia_proton-0%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_proton-0%d.h5',j), '/x_part_p',[s3 1])
            h5create(sprintf('elimaia_proton-0%d.h5',j), '/y_part_p', [s3 1])
            h5write(sprintf('elimaia_proton-0%d.h5',j), '/x_part_p', xx_p)
            h5write(sprintf('elimaia_proton-0%d.h5',j), '/y_part_p', yy_p)
            h5write(sprintf('elimaia_proton-0%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_proton-0%d.h5',j), '/energy_p', energy_p)
            
            h5create(sprintf('elimaia_carbon-0%d.h5',j),'/energy_c',[s4 1])
            h5create(sprintf('elimaia_carbon-0%d.h5',j),'/time',[1])
            h5create(sprintf('elimaia_carbon-0%d.h5',j), '/x_part_c',[s4 1])
            h5create(sprintf('elimaia_carbon-0%d.h5',j), '/y_part_c', [s4 1])
            h5write(sprintf('elimaia_carbon-0%d.h5',j), '/x_part_c', xx_c)
            h5write(sprintf('elimaia_carbon-0%d.h5',j), '/y_part_c', yy_c)
            h5write(sprintf('elimaia_carbon-0%d.h5',j),'/time',cas)
            h5write(sprintf('elimaia_carbon-0%d.h5',j), '/energy_c', energy_c) 
            
           
  
  
          
        close
    end 
end

  

end %fce

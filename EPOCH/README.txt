This folder contains all the codes and the scripts useful to simulate laser-target interaction with EPOCH. In particular:

	1) folder “conversionPicSdfFile2csv” contains: 
		
        1.1) folder “materials_ELI” which contains:

            1.1.1) folder "matlab_scripts"

                1.1.1.1) csvwrite_with_headers.m: matlab function which allows to build a .csv file with a row of headers
                1.1.1.2) sdf2csvFileConversion.m: Cirrone's version
                1.1.1.3) Exportdoh5_M.m: matlab function tha makes a movie (by using scratch) with the output images
                1.1.1.4) folder "functions" which contains matlab function useful to read .sdf files

            1.1.2) two .sdf files
            1.1.3) .csv file obtained from the conversion of p0211.sdf file

		1.2) folder “read_plot_convert_sdf2csv” which contains:

			1.2.1) folder "csv_files" which contains the results of .sdf files conversion
            1.2.2) folder "images" which contains some plots obtained with matlab in order to compre them with VisIt outputs
            1.2.3) folder "matlab_scripts":
                1.2.3.1) folder "functions" which contains matlab function useful to read .sdf files and to write .csv files
                1.2.3.2) general_conversion_1D.m: matlab script to convert .sdf file into .csv file in 1D (phase-space)
                1.2.3.3) general_conversion.m: general matlab script to convert .sdf file into .csv file
                1.2.3.4) sdf2csv_test.m: is the same of sdf2csvFileConversion.m (Cirrone's version) with some modifications
                                         (Cagni's test version)

	2) folder “ReadCSVFileFromPIC” contains:
 
        2.1) readAndAnalizeCSVFileFromPIC.m: matlab script that reads and plots data from .csv files obtained with the
                                             .sdf to .csv conversion script
        2.2) csvwrite_with_headers.m: matlab function which allows to build a .csv file with a row of headers
        2.3) some .csv files

	3) folder “materiale_belfast” contains: 

		3.1) folder "input_decks":

            3.1.1)folder “no_pre_plasma” which contains:

			    3.1.1.1) step-like density profile of a neutral hydrogen plasma input.deck

		    3.1.2) folder “pre_plasma” which contains:

			    3.1.2.1) pre-plasma input.deck

		3.2) folder “python_scripts” which contains:

			3.2.1) catania_1d.py: it is a python script wich reads all the .sdf outpus of a simulation and makes a movie
                                  of the simulation
            3.2.2) make_SDF_movie: it is a python function that permits to make a movie with the .png files, each coming 
                                   from their respective .sdf file

        3.3) EPOCH.docx: instruction for beginners

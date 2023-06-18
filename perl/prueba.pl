#!/usr/bin/perl -w
# $numStr_ = 2;
# $pBuss_ = 50;
# $numSim_ = 180;

	# $path = "xxresults/stats/";
	# $numStr = sprintf("%03d",$numStr_);
	# $pBuss = sprintf("%03d",$pBuss_);
	# $numSim = sprintf("%03d",$numSim_);
	
	# $command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$pBuss."_".$numSim.".out";
	
	# print($command);

	# system("ls");
	# #system($command);
	
	# if ( $? == -1 )
	# {
		# print "\nOrden fallida: $!\n";
	# }
	# elsif (($? >> 8) != 0)
	# {
		# print "\nError en parametros de la orden\n";
		# die "Please fix the wrong parameter";
	  # #printf "Orden finalizo con valor %d\n", $? >> 8;
	# }
	
	# print("\n",$command,"\n");
	
	use Math::Round;
	$float=1.75;
	my $rounded = round( $float );
	printf "valor %.2f\n", $rounded;

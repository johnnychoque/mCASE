#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-users.cfg";
$startSim = 1;
$endSim = 20;

sub changeSimulationParam
{
    my($numSimulation_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>) {
		if($_ =~ /\[*\]/) {
			if($_ =~ /SNAPSHOTS/) {
				$flag_ = 1; 
			} else {
				$flag_ = 0;
			}
		}

		if($_ =~ /FILE=/ && $flag_ == 1) {
			$aux = sprintf("FILE=%03d\n",$numSimulation_);
			print OUT_FILE $aux;
		} else {
			print OUT_FILE $_;
		}

    }
	
    close(CFG_FILE);
    close(OUT_FILE);
    $command = "rm -f ".$cfgFile;
    system($command);
    $command = "mv ".$tempFILE." ".$cfgFile;
    system($command);
}

sub executeSimulation
{
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);
}


# The main!!!
$t_ini = localtime;

for($i = $startSim ; $i <= $endSim ; $i++) {
	print "\n", "Simulation ",$i, "\n";
	&changeSimulationParam($i);
	&executeSimulation();
}


$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";

#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-jitel.cfg";
$totSimulation_ = 100;
$totStrategies_ = 11;

@prefOP_  = (0.25, 1.00, 0.00, 0.00, 0.00, 0.50, 0.50, 0.50, 0.00, 0.00, 0.00);
@handover_= (0.25, 0.00, 1.00, 0.00, 0.00, 0.50, 0.00, 0.00, 0.50, 0.50, 0.00);
@quality_ = (0.25, 0.00, 0.00, 1.00, 0.00, 0.00, 0.50, 0.00, 0.50, 0.00, 0.50);
@load_    = (0.25, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.50, 0.00, 0.50, 0.50);

sub changeAccessSelectionStrategy
{
    my($prefOP_,$handover_,$quality_,$load_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /CONSTRAINTS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /OPERATOR=/ && $flag_ == 1) {
			$aux = sprintf("OPERATOR=%.2f\n",$prefOP_);
			print OUT_FILE $aux;
		} elsif($_ =~ /HANDOVER=/ && $flag_ == 1) {
			$aux = sprintf("HANDOVER=%.2f\n",$handover_);
			print OUT_FILE $aux;
		} elsif($_ =~ /QUALITY=/ && $flag_ == 1) {
			$aux = sprintf("QUALITY=%.2f\n",$quality_);
			print OUT_FILE $aux;
		} elsif($_ =~ /LOAD=/ && $flag_ == 1) {
			$aux = sprintf("LOAD=%.2f\n",$load_);
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

sub changeSimulationNumbers
{
    my($numStrategy_, $numSimulation_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>) {
		if($_ =~ /\[*\]/) {
			if($_ =~ /SIMULATION/) {
				$flag_ = 1; 
			} else {
				$flag_ = 0;
			}
		}

		if($_ =~ /STRATEGY=/ && $flag_ == 1) {
			$aux = sprintf("STRATEGY=%03d\n",$numStrategy_);
			print OUT_FILE $aux;
		} elsif($_ =~ /FILE=/ && $flag_ == 1) {
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
    my($currenti_) = @_;
    my($command);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);
}

sub moveResults
{
    my($numSim_,$numStra_) = @_;
    my($command_, $fileRoot_, $fileSrc_,$k_);
    
    $fileSrc_ = sprintf("%03d_datos_matlab.txt",$numSim_);	
    $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_datos_matlab.txt",$numStra_+5,$numStra_+5,$numSim_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_bs_matlab.txt",$numSim_);
    $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_bs_matlab.txt",$numStra_+5,$numStra_+5,$numSim_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_users_matlab.txt",$numSim_);
    $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_users_matlab.txt",$numStra_+5,$numStra_+5,$numSim_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    for($k_ = 1 ; $k_ <= 200; $k_++) {
    	$fileSrc_ = sprintf("%03d_final_solution_%04d.txt",$numSim_,$k_);
    	$fileRoot_ = sprintf("strategy_%02d/%02d_%03d_final_solution_%04d.txt",$numStra_+5,$numStra_+5,$numSim_,$k_);
    	$command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    	system($command_);

	$fileSrc_ = sprintf("%03d_qos_final_%04d.txt",$numSim_,$k_);
        $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_qos_final_%04d.txt",$numStra_+5,$numStra_+5,$numSim_,$k_);
        $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
        system($command_);
    }
}

sub flushLogFiles
{
    my($currenti_) = @_;
    my($command_, $fileRoot_);
    
    $fileRoot_ = sprintf("%03d_printOutput_*.cfg",$currenti_);
    $command_ = "rm -f ".$fileRoot_;
    system($command_);

    $fileRoot_ = sprintf("%03d_solglpk_*.txt",$currenti_);
    $command_ = "rm -f ".$fileRoot_;
    system($command_);

    $fileRoot_ = sprintf("genera_mat_rest_*.cfg");
    $command_ = "rm -f ".$fileRoot_;
    system($command_);

    $fileRoot_ = sprintf("%03d_qos_red_*.txt",$currenti_);
    $command_ = "rm -f ".$fileRoot_;
    system($command_);
}

# The main!!!
$t_ini = localtime;

for($j = 1 ; $j <= $totStrategies_ ; $j++) {
	&changeAccessSelectionStrategy($prefOP_[$j-1], $handover_[$j-1], $quality_[$j-1], $load_[$j-1]);
	# I go through all cases
	for($i = 1 ; $i <= $totSimulation_ ; $i++) {
		printf("\n\nRunning mCASE for strategy %d simulation %d\n",$j,$i);
		&changeSimulationNumbers($j,$i);
		&executeSimulation($i);
#		&moveResults($i,$j);
#		&flushLogFiles($i);
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";

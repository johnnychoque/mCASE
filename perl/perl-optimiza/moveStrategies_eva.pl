#!/usr/bin/perl -w

$numTraces_ = 10;
$numStrategies_ = 4;
@strategies_ = (1, 3, 4, 5);

sub moveResults
{
    my($currenti_,$currentj_) = @_;
    my($command_, $fileSrc_, $fileDst_,$k_);
    
    #$fileSrc_ = sprintf("%03d_datos_matlab.txt",$currenti_);
    $fileSrc_ = sprintf("results/strategy_eva_%02d/%03d_datos_matlab.txt",$currentj_,$currenti_);
    $fileDst_ = sprintf("results/strategy_%02d/%02d_%03d_datos_matlab.txt",$currentj_,$currentj_,$currenti_);
    $command_ = "mv ".$fileSrc_." ".$fileDst_;
    system($command_);

    #$fileSrc_ = sprintf("%03d_bs_matlab.txt",$currenti_);
    $fileSrc_ = sprintf("results/strategy_eva_%02d/%03d_bs_matlab.txt",$currentj_,$currenti_);
    $fileDst_ = sprintf("results/strategy_%02d/%02d_%03d_bs_matlab.txt",$currentj_,$currentj_,$currenti_);
    $command_ = "mv ".$fileSrc_." ".$fileDst_;
    system($command_);

    #$fileSrc_ = sprintf("%03d_users_matlab.txt",$currenti_);
    $fileSrc_ = sprintf("results/strategy_eva_%02d/%03d_users_matlab.txt",$currentj_,$currenti_);
    $fileDst_ = sprintf("results/strategy_%02d/%02d_%03d_users_matlab.txt",$currentj_,$currentj_,$currenti_);
    $command_ = "mv ".$fileSrc_." ".$fileDst_;
    system($command_);

    for($k_ = 1 ; $k_ <= 200; $k_++) {
    	#$fileSrc_ = sprintf("%03d_final_solution_%04d.txt",$currenti_,$k_);
    	$fileSrc_ = sprintf("results/strategy_eva_%02d/%03d_final_solution_%04d.txt",$currentj_,$currenti_,$k_);
	$fileDst_ = sprintf("results/strategy_%02d/%02d_%03d_final_solution_%04d.txt",$currentj_,$currentj_,$currenti_,$k_);
    	$command_ = "mv ".$fileSrc_." ".$fileDst_;
    	system($command_);

	#$fileSrc_ = sprintf("%03d_qos_final_%04d.txt",$currenti_,$k_);
        $fileSrc_ = sprintf("results/strategy_eva_%02d/%03d_qos_final_%04d.txt",$currentj_,$currenti_,$k_);
	$fileDst_ = sprintf("results/strategy_%02d/%02d_%03d_qos_final_%04d.txt",$currentj_,$currentj_,$currenti_,$k_);
        $command_ = "mv ".$fileSrc_." ".$fileDst_;
        system($command_);
    }
}

# The main!!!

for($j = 1 ; $j <= $numStrategies_ ; $j++) {
	# I go through all cases
	for($i = 1 ; $i <= $numTraces_ ; $i++) {
   		&moveResults($i,$strategies_[$j-1]);
	}
}

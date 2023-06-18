#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

int main(void)
{
	string currSnapshotsFilename;
	char filenameSuffix[4], filenamePreffix[3];
	int m_ssCounter;
	FILE *UTSnapshotsFile;
	int utID, termTypeID, userTypeID, myOperator, s1, s2, s3, numUsers;
	int c1=1, c2=1, c3=3, tempLoad,i,x, numSnapshots=360, numParams, numSim=1, systemCapacity=4*20;
	//int c1=1, c2=1, c3=2, tempLoad,i,x, numSnapshots=100, numParams, numSim=20, systemCapacity=100;
	float ssLoad;
	float m_x, m_y;
	bool endLoop;

	for (x=1; x<=numSim; x++) {
		ssLoad = 0;
		for (i=1; i<=numSnapshots; i++) {
			sprintf(filenamePreffix,"%03d",x);
			sprintf(filenameSuffix,"%04d",i);
			currSnapshotsFilename = "../results/snapshots/" + string(filenamePreffix) + "_UT_" + string(filenameSuffix) + ".txt";
			UTSnapshotsFile = fopen(currSnapshotsFilename.c_str(), "r");
			numUsers=0;
			//tempLoad=0;
			endLoop=false;
			do {
				numParams = fscanf(UTSnapshotsFile, "%d %f %f %d %d %d %d %d %d",
					&utID, &m_x, &m_y, &termTypeID, &userTypeID, &myOperator, &s1, &s2, &s3);
				if (numParams == 9) {
					numUsers++;
					//tempLoad = tempLoad + c1*s1 + c2*s2 + c3*s3;
					ssLoad = ssLoad + c1*s1 + c2*s2 + c3*s3;
				}
				else {
					endLoop = true;
				}		
			} while (!endLoop);
			
			fclose(UTSnapshotsFile);
			//ssLoad = ssLoad + (float)tempLoad/numUsers;
			//cout << "file " << i << " numUsers " << numUsers << " ssLoad " << ssLoad << endl;
			
		}
		
		//cout << "Num. snapshots: " << numSnapshots << endl;
		//cout << "Num. users: " << numUsers << endl;
		cout << "Num. simulation: " << x << endl;
		cout << "Load avg. users: " << ssLoad/numSnapshots << endl;
		cout << "Load avg. system: " << ssLoad/(numSnapshots*systemCapacity) << endl;
	}
}	
	
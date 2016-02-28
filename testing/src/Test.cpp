#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "TestMatrix.h"
#include "TestFilter.h"
#include "TestTaskMgr.h"
#include "TestJoystickHelper.h"
#include "TestPoseManager.h"

void runAllTests(int argc, char const *argv[]){
	cute::suite s;

	s += TestMatrix().MakeSuite();
	s += TestFilters().MakeSuite();
	s += TestTaskMgr().MakeSuite();
	s += TestJoystickHelper().MakeSuite();
	s += TestPoseManager().MakeSuite();

	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}

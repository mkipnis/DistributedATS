## GenTools

QuickFIX2FastDDS.py converts specified in the requirements file messages, groups, and fields from the FIX specs to the FastDDS IDL libraries and conversion helper classes.

cd QuickFIX2FastDDS
QuickFIX2FastDDS.py -r <requirements> -s <fix_specs>
 
Example:
```
python QuickFIX2FastDDS.py -r ../spec/dats_requirements.xml -s ../../FIXGateway/spec/FIX44.xml
```

To regenerate hpp and cpp files from idl
-----------------------------------------
git clone https://github.com/eProsima/Fast-DDS-Gen.git
cd Fast-DDS-Gen
./gradlew assemble

cd ..
run_fastddsgen.sh

## GenTools

QuickFIX2OpenDDS.py converts specified in the requirements file messages, groups, and fields from the FIX specs to the OpenDDS IDL libraries and conversion helper classes.

QuickFIX2OpenDDS/QuickFIX2OpenDDS.py -r <requirements> -s <fix_specs>
 
Example:

python QuickFIX2OpenDDS.py -r ../spec/dats_requirements.xml -s ../../FIXGateway/spec/FIX44.xml

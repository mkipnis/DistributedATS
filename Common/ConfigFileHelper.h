#ifndef __CONFIG_FILE_HELPER
#define __CONFIG_FILE_HELPER

#include <ace/Configuration.h>
#include <ace/Configuration_Import_Export.h>


class ConfigFileHelper
{
  
       
       public:
                ConfigFileHelper( const ACE_TCHAR* config_file_name )
                {
                        m_config.open();

                        ACE_Ini_ImpExp ini_config( m_config );

                        ini_config.import_config( config_file_name );

                        m_config_root = m_config.root_section();


                };

                int get_string_value( const ACE_TCHAR* section_name, const ACE_TCHAR* parameter_name, ACE_TString& value )
                {
                        ACE_Configuration_Section_Key section;

                        if ( m_config.open_section ( m_config_root, section_name, 1, section ) )
                        {
                                return ACE_OS::last_error();
                        }

                        if ( m_config.get_string_value ( section, parameter_name, value) )
                        {
                                return ACE_OS::last_error();
                        };

                        return 0;
                };
                
                int get_integer_value( const ACE_TCHAR* section, const ACE_TCHAR* name, int* int_value)
                {
                        ACE_TString StringValue;
                        ACE_TCHAR pszString[30];
                        ACE_OS::strcpy (pszString, ACE_TEXT ("0"));
                        int IntegerValue = 0;

                        if ( get_string_value( section, name, StringValue ) )
                        {
                                return ACE_OS::last_error();
                        }

                        ACE_OS::strncpy (pszString, StringValue.c_str (), 30);

                        IntegerValue = ACE_OS::atoi (pszString);
                        *int_value = IntegerValue;

                        return 0;

                };

        protected:
                ConfigFileHelper();

        private:
                ACE_Configuration_Heap m_config;
                ACE_Configuration_Section_Key m_config_root;
};

#endif

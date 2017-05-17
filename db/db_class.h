#ifndef DB_CLASS_H
#define DB_CLASS_H
#include "db_conn_class.h"
#include "db_table_class.h"
#include "db_request.h"

template <class T>
class db
{
    
    virtual std::vector<T> get_data() = 0;
    virtual std::string class_name() = 0;
    virtual std::string group_by() = 0; 
    
private:

    db_connection connection;
    std::string transform_ids_to_string( std::vector<int> id ); 
    
//protected:


public:
        
    std::shared_ptr<PGresult> select_res;
    std::vector<drugstore> request_drugstores( std::vector<int> drugstore_ids );
    std::vector<drug> request_drugs( std::vector<int> drug_ids );
    std::vector<distance> request_distances( std::vector<drugstore> drugstores );
    std::vector<relation> request_relations_with_pills( std::vector<drug> drugs );
    std::vector<T> request_data();
    
    
    void insert_drugstores( std::vector<drugstore> drugstores );
    
    
};

template <class T>
class drugs_request: public db<T>
{
    std::vector<drug> get_data();
    std::string class_name();    
    std::string group_by();
};

template <class T>
class relations_request: public db<T>
{
    std::vector<relation> get_data();
    std::string class_name();  
    std::string group_by(); 
};

template <class T>
class drugstores_request: public db<T>
{
    std::vector<drugstore> get_data();
    std::string class_name();
    std::string group_by();   
};

template <class T>
class distances_request: public db<T>
{
    std::vector<distance> get_data();
    std::string class_name(); 
    std::string group_by();      
};

template <class T>
class drugstores_no_coord_request: public db<T>
{
    std::vector<drugstore_no_coordinates> get_data();
    std::string class_name();      
    std::string group_by(); 
};


template <class T>
std::string db<T>::transform_ids_to_string( std::vector<int> id )
{
    
    size_t length = id.size();
    std::string request = " id = ";
    if ( length > 0)
        request += std::to_string(id[0]);
        
    for (size_t i=1 ; i < length; i++)
        request += " OR id = "+std::to_string(id[i]);
    
    return request;
    
}

template <class T>
std::vector<drugstore> db<T>::request_drugstores( std::vector<int> drugstore_ids )
{
    
    std::ifstream request_file;
    
    size_t n_rows;
    size_t length;
    
	const size_t n = 256;
    
	char *buf =  new char[n];
    std::string request;
    std::string file_name;
    
    std::vector<drugstore> drugstores;
    
    length = drugstore_ids.size();
    file_name = "requests/request_drugstores.txt";
        
    request_file.open( file_name );        //Getting request from file
    
    if ( request_file.is_open() )
		std::cout<<"Opened request file"<<std::endl;
        
	else
	{
        delete buf;
		throw std::runtime_error("Unable to open request file");
	}

    request_file.getline( buf, 256 );
                        
    request = buf;
    request += transform_ids_to_string( drugstore_ids );
    
    delete buf;
    
    select_res.reset( PQexec( connection.get_connection().get(), request.c_str() ), &PQclear );
    
    n_rows = PQntuples( select_res.get() );
    
    if ( !( PQresultStatus( select_res.get() ) == PGRES_TUPLES_OK && ( n_rows == length) ) )
        throw std::runtime_error( PQresStatus( PQresultStatus ( select_res.get() )) );             //Checking for bad tuples

    drugstores = get_drugstores( select_res );
    
    select_res.reset();
    
    return drugstores;
    
}


template <class T>
std::vector<drug> db<T>::request_drugs( std::vector<int> drug_ids )
{
    
    std::ifstream request_file;
    
    size_t n_rows;
    size_t length;
    
	const size_t n = 256;
    
	char *buf =  new char[n];
    std::string request;
    std::string file_name;

    std::vector<drug> drugs;
    
    length = drug_ids.size();

    file_name = "requests/request_drugs.txt";
        
    request_file.open( file_name );        //Getting request from file
    
    if ( request_file.is_open() )
		std::cout<<"Opened request file"<<std::endl;
        
	else
	{
        delete buf;
		throw std::runtime_error("Unable to open request file");
	}

    request_file.getline( buf, 256 );
                        
    request = buf;
    request += transform_ids_to_string( drug_ids );
    
    delete buf;
    
    select_res.reset( PQexec( connection.get_connection().get(), request.c_str() ), &PQclear );
    
    n_rows = PQntuples( select_res.get() );
    
    if ( !( PQresultStatus( select_res.get() ) == PGRES_TUPLES_OK && ( n_rows == length) ) )
        throw std::runtime_error( PQresStatus( PQresultStatus ( select_res.get() )) );     //Checking for bad tuples

    drugs = get_drugs( select_res );

    select_res.reset();
    
    return drugs;
    
}


template <class T>
std::vector<distance> db<T>::request_distances( std::vector<drugstore> drugstores )
{
    
    std::ifstream request_file;
    
    size_t n_rows;
    size_t length;
    
	const size_t n = 256;
    
	char *buf =  new char[n];
    std::string request;
    std::string file_name;
    
    std::vector<distance> distances;
    
    length = drugstores.size();
    
    std::vector<int> drugstore_ids (length);
    
    for (size_t i = 0; i < length; i++)
        drugstore_ids[i] = drugstores[i].get_id();
    
    file_name = "requests/request_distances.txt";
        
    request_file.open( file_name );        //Getting request from file
    
    if ( request_file.is_open() )
		std::cout<<"Opened request file"<<std::endl;
        
	else
	{
        delete buf;
		throw std::runtime_error("Unable to open request file");
	}

    request_file.getline( buf, 256 );
                        
    request = buf;
    request += transform_ids_to_string( drugstore_ids );
    
    delete buf;
    
    select_res.reset( PQexec( connection.get_connection().get(), request.c_str() ), &PQclear );
    
    n_rows = PQntuples( select_res.get() );
    
    if ( !( PQresultStatus( select_res.get() ) == PGRES_TUPLES_OK && ( n_rows == length) ) )
        throw std::runtime_error( PQresStatus( PQresultStatus ( select_res.get() )) );        //Checking for bad tuples

    distances.resize(n_rows);
    distances = get_distances( select_res );
    
    select_res.reset();
    
    return distances;
    
}

template <class T>
std::vector<relation> db<T>::request_relations_with_pills( std::vector<drug> drugs )
{
    
    std::ifstream request_file;
    
    size_t n_rows;
    size_t length;
	const size_t n = 256;
    
	char *buf =  new char[n];
    std::string request;
    std::string file_name;
    
    std::vector<relation> relations;
    
    length = drugs.size();
    
    std::vector<int> drug_ids (length);
    
    for (size_t i = 0; i < length; i++)
        drug_ids[i] = drugs[i].get_id();
    
    file_name = "requests/request_relations.txt";
        
    request_file.open( file_name );        //Getting request from file
    
    if ( request_file.is_open() )
		std::cout<<"Opened request file"<<std::endl;
        
	else
	{
        delete buf;
		throw std::runtime_error("Unable to open request file");
	}

    request_file.getline( buf, 256 );
                        
    request = buf;
    request += transform_ids_to_string( drug_ids );
    
    delete buf;
    
    select_res.reset( PQexec( connection.get_connection().get(), request.c_str() ), &PQclear );
    
    n_rows = PQntuples( select_res.get() );
    
    if ( !( PQresultStatus( select_res.get() ) == PGRES_TUPLES_OK && ( n_rows == length) ) )
        throw std::runtime_error( PQresStatus( PQresultStatus ( select_res.get() )) );        //Checking for bad tuples

    relations.resize(n_rows);
    relations = get_relations( select_res );
    
    select_res.reset();
    
    return relations;
    
}


// insert methods
template <class T>
void db<T>::insert_drugstores( std::vector<drugstore> drugstores )
{

    std::ifstream request_file;
    
    std::string transaction;
    std::string request;
    std::string file_name;
    
    std::size_t n;
    
    n = 256;
    
    char *buf =  new char[n];

    if ( drugstores.size() > 0 )
        transaction = drugstores[0].to_string();
        
    for ( size_t i = 1; i < drugstores.size(); i++ )
    {
        transaction += (" " + drugstores[i].to_string() );
    }
    
    
    file_name = "requests/transaction_drugstores.txt";
        
    request_file.open( file_name );
    
    if ( request_file.is_open() )
		std::cout<<"Opened request file"<<std::endl;
        
	else
	{
        delete buf;
		throw std::runtime_error("Unable to open request file");
	}

    request_file.getline( buf, n );
                        
    request = buf;
    request += transaction;
    
    select_res.reset( PQexec( connection.get_connection().get(), request.c_str() ), &PQclear );
        
    if ( !( PQresultStatus( select_res.get() ) == PGRES_TUPLES_OK ) )
        throw std::runtime_error( PQresStatus( PQresultStatus ( select_res.get() )) );  
        
    delete buf;
}


template <class T>
std::vector<T> db<T>::request_data()
{
    
    std::ifstream request_file;
    
    size_t n_rows;
    
	const size_t n = 256;
    
	char *buf =  new char[n];
    
    std::string request;
    std::string file_name =  class_name();    
    std::string group = group_by();
    
    file_name = "requests/" + class_name() + "_all.txt";
        
    request_file.open( file_name );        
    
    if ( request_file.is_open() )
		std::cout<<"Opened request file"<<std::endl;
        
	else
	{
        delete buf;
		throw std::runtime_error("Unable to open request file");
	}

    request_file.getline( buf, 256 );
    
    request = buf;
    
    request += group_by();
    
    delete buf;
    
    select_res.reset( PQexec( connection.get_connection().get(), request.c_str() ), &PQclear );
    
    n_rows = PQntuples( select_res.get() );
    
    if ( !( PQresultStatus( select_res.get() ) == PGRES_TUPLES_OK && n_rows ) )
        throw std::runtime_error( PQresStatus( PQresultStatus ( select_res.get() )) );      //Checking for bad tuples
    
    std::vector<T> data = get_data();

    select_res.reset();
    
    return data;
    
}

template <class T>
std::vector<drug> drugs_request<T>::get_data()
{
    return get_drugs( this->select_res );
}

template <class T>
std::string drugs_request<T>::class_name()
{
    return "drugstore";
}

template <class T>
std::string drugs_request<T>::group_by()
{
    return "";
}

template <class T>
std::vector<relation> relations_request<T>::get_data()
{
    return get_drugs( this->select_res );
}

template <class T>
std::string relations_request<T>::class_name()
{
    return "relation";
}

template <class T>
std::string relations_request<T>::group_by()
{
    return "";    
}

template <class T>
std::vector<drugstore> drugstores_request<T>::get_data()
{
    return get_drugstores( this->select_res );
}

template <class T>
std::string drugstores_request<T>::class_name()
{
    return "drugstore";
}

template <class T>
std::string drugstores_request<T>::group_by()
{
    return "";    
}

template <class T>
std::vector<distance> distances_request<T>::get_data()
{
    return get_drugs( this->select_res );
}
 
template <class T>   
std::string distances_request<T>::class_name()
{
    return "distance";
}

template <class T>
std::string distances_request<T>::group_by()
{
    return "";    
}

template <class T>
std::vector<drugstore_no_coordinates> drugstores_no_coord_request<T>::get_data()
{
    return get_drugs( this->select_res );
}

template <class T>
std::string drugstores_no_coord_request<T>::class_name()
{
    return "drugstore_no_coordinates";
}

template <class T>
std::string drugstores_no_coord_request<T>::group_by()
{
    return "";    
}
#endif
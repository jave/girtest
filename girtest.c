//license:GPL v3
//author:joakim verona
#include <girepository.h>
#include <stdio.h>

void
print_attributes (GIBaseInfo *info)
{
  GIAttributeIter iter = { 0, };
  char *name;
  char *value;
  while (g_base_info_iterate_attributes (info, &iter, &name, &value))
    {
      g_print ("attribute name: %s value: %s", name, value);
    }
}

#define XX(format, arg) (printf("%*s " format " \n", 2*level, " ", (arg)))

void dumpme (GIBaseInfo *base_info, int level)
{

  
  int numtype = g_base_info_get_type  (  base_info);
  if(numtype!=GI_INFO_TYPE_TYPE){
    //its not partucularily clear why get_name cant return something for a type,
    //but its the way it is
    XX("name:%s", g_base_info_get_name ( base_info));
  }
  XX("type:%s", g_info_type_to_string (g_base_info_get_type (base_info)));

  //print_attributes (base_info);

  int i=0;
  int N=0;
  int typetag=0;
    char* s;
    
  switch(numtype){
  case GI_INFO_TYPE_FUNCTION:
    XX("args:%d",    N=g_callable_info_get_n_args (base_info));
    for (i = 0; i < N; ++i)
      {
        XX("arg:%d", i);
        dumpme(    g_callable_info_get_arg  (base_info, i), level + 1);   
      }

    GIFunctionInfoFlags gif = g_function_info_get_flags (base_info);
    XX("flags:%d",gif);
    //XX("        symbol:%s\n", g_function_info_get_symbol (base_info));
    break;
  case GI_INFO_TYPE_ARG:

    switch(g_arg_info_get_direction(base_info)){
      case GI_DIRECTION_IN: s="In";break;
      case GI_DIRECTION_OUT: s="Out";break;
      case GI_DIRECTION_INOUT: s="InOut";break;}
    XX("direction:%s",            s);
    XX("is_caller_allocates:%s",      g_arg_info_is_caller_allocates      (base_info) ? "Yes":"No");
    XX("is_return_value:%s",          g_arg_info_is_return_value          (base_info) ? "Yes":"No");
    XX("is_optional:%s",              g_arg_info_is_optional              (base_info) ? "Yes":"No");
    XX("may_be_null:%s",              g_arg_info_may_be_null              (base_info) ? "Yes":"No");
    XX("ownership_transfer:%d",   g_arg_info_get_ownership_transfer   (base_info));
    XX("scope:%d",                g_arg_info_get_scope                (base_info));
    XX("closure:%d",              g_arg_info_get_closure              (base_info));
    XX("destroy:%d",              g_arg_info_get_destroy              (base_info));
    dumpme(g_arg_info_get_type(base_info), level+1);
      
    break;
  case   GI_INFO_TYPE_TYPE:
    //const gchar *       g_type_tag_to_string                (GITypeTag type);

    XX("tag:%d",                   typetag=g_type_info_get_tag                 (base_info));
    XX("tag name:%s",g_type_tag_to_string (typetag));
    XX("is_pointer:%d",                g_type_info_is_pointer              (base_info));
    /* GITypeInfo *        g_type_info_get_param_type          (base_info, */
    /*                                                          gint n); */
    if(GI_TYPE_TAG_INTERFACE == typetag){
      //this leads to tremendous redundancy
      //dumpme(g_type_info_get_interface           (base_info), level+1); 
    }
    if(typetag== GI_TYPE_TAG_ARRAY){
      XX("array_length:%d",          g_type_info_get_array_length        (base_info));
      XX("array_fixed_size:%d",      g_type_info_get_array_fixed_size    (base_info));
      XX("is_zero_terminated:%d",        g_type_info_is_zero_terminated      (base_info));
      XX("array_type:%d",            g_type_info_get_array_type          (base_info));
    }
    break;
  case  GI_INFO_TYPE_OBJECT:
    //g_object_info_get_type_name (base_info);
    //g_object_info_get_type_init ();
    XX("abstract:%d", g_object_info_get_abstract (base_info));
    //g_object_info_get_fundamental ();
    //g_object_info_get_parent
    XX("interfaces:%d",N=g_object_info_get_n_interfaces (base_info));
    for (i = 0; i < N; ++i)
      {
        XX("interface:%d", i);
        dumpme(g_object_info_get_interface (base_info, i), level + 1);   
              
      }
    XX("fields:%d",N=g_object_info_get_n_fields (base_info));
    for (i = 0; i < N; ++i)
      {
        XX("field:%d", i);
        dumpme(g_object_info_get_field (base_info, i), level + 1);   
              
      }
    XX("properties:%d",g_object_info_get_n_properties (base_info));
    //g_object_info_get_property ()
    XX("methods:%d",N=g_object_info_get_n_methods (base_info));
    for (i = 0; i < N; ++i)
      {
        XX("method:%d", i);
        dumpme(g_object_info_get_method (base_info, i), level + 1);
      }
    //
    XX("signals:%d",g_object_info_get_n_signals (base_info));
    //g_object_info_get_signal ()
    XX("vfuncs:%d", g_object_info_get_n_vfuncs (base_info));
    //g_object_info_get_vfunc ()
    break;      
  case  GI_INFO_TYPE_INTERFACE:
        XX("imethods:%d",N=g_interface_info_get_n_methods  (base_info));
    break;
  case GI_INFO_TYPE_STRUCT:
    XX("nfields:%d",N=g_struct_info_get_n_fields (base_info));
    for (i = 0; i < N; ++i)
      {
        XX("field:%d", i);
        dumpme((GIBaseInfo *)g_struct_info_get_field(base_info, i), level + 1);
      }  
    break;
    case GI_INFO_TYPE_FIELD:
/*       GIFieldInfoFlags    g_field_info_get_flags              (GIFieldInfo *info); */
/* gint                g_field_info_get_size               (GIFieldInfo *info); */
/* gint                g_field_info_get_offset             (GIFieldInfo *info); */
/* GITypeInfo *        g_field_info_get_type               (GIFieldInfo *info); */
      break;
  case     GI_INFO_TYPE_ENUM         :
    XX("nenums:%d", N=g_enum_info_get_n_values            (base_info)); 
/* GIValueInfo *       g_enum_info_get_value               (GIEnumInfo *info, */
/*                                                          gint n); */
/* gint                g_enum_info_get_n_methods           (GIEnumInfo *info); */
/* GIFunctionInfo *    g_enum_info_get_method              (GIEnumInfo *info, */
/*                                                          gint n); */
/* GITypeTag           g_enum_info_get_storage_type        (GIEnumInfo *info); */
/* gint64              g_value_info_get_value              (GIValueInfo *info); */
    break;
    
    /* XX("   type init :%s \n", g_object_info_get_type_init     (base_info)); */
    /* GI_INFO_TYPE_INVALID, */
    /*  GI_INFO_TYPE_CALLBACK, */
    /*  GI_INFO_TYPE_STRUCT, */
    /*  GI_INFO_TYPE_BOXED, */

    /*  GI_INFO_TYPE_FLAGS, */

    /*  GI_INFO_TYPE_CONSTANT, */
    /*  GI_INFO_TYPE_INVALID_0,    /\* 10 *\/  /\** DELETED - used to be ERROR_DOMAIN **\/ */
    /*  GI_INFO_TYPE_UNION, */
    /*  GI_INFO_TYPE_VALUE, */
    /*  GI_INFO_TYPE_SIGNAL, */
    /*  GI_INFO_TYPE_VFUNC, */
    /*  GI_INFO_TYPE_PROPERTY,     /\* 15 *\/ */

    /*  , */
    /*  GI_INFO_TYPE_TYPE, */
    /*  GI_INFO_TYPE_UNRESOLVED */

  }

}

int main(int argc, char *argv[])
{
  
  GIRepository *repository;
  g_type_init();
  GError *error = NULL;
  GIBaseInfo *base_info;
  GIArgument in_args[5];
  GIArgument retval;

  char* namespace = "Gtk";
  char* namespace_version = "3.0";
    
  repository = g_irepository_get_default();
  g_irepository_require(repository, namespace, namespace_version, 0, &error);
  if (error) {
    g_error("1ERROR: %s\n", error->message);
    return 1;
  }
    
  /* base_info = g_irepository_find_by_name(repository, namespace, "Button"); */
  /* if (!base_info) { */
  /*   g_error("2ERROR: %s\n", "Could not find GLib.warn_message"); */
  /*   return 1; */
  /* } */

  int numinfo= g_irepository_get_n_infos (repository, namespace);
  int i=0;
  //how many enries are there in the namespace?
  printf("num info:%d \n",numinfo);
  //dump each entry in the namespace
  for ( i = 0; i < numinfo; ++i)
    {
      base_info = g_irepository_get_info(repository, namespace, i);
      dumpme(base_info, 0);
      g_base_info_unref(base_info);       

    }

    

  //printf("typename :%s \n", g_info_type_to_string (    g_base_info_get_type (base_info)));
    

    



    
  /* in_args[0].v_pointer = "domain"; */
  /* in_args[1].v_pointer = "glib-print.c"; */
  /* in_args[2].v_pointer = "30"; */
  /* in_args[3].v_pointer = "main"; */
  /* in_args[4].v_pointer = "hello world"; */

  /* if (!g_function_info_invoke ((GIFunctionInfo *)base_info, */
  /*                              (const GIArgument*)&in_args, */
  /*                              5, */
  /*                              NULL, */
  /*                              0, */
  /*                              &retval, */
  /*                              &error)) { */
  /*   g_error("3ERROR: %s\n", error->message); */
  /*   return 1; */
  /* } */

  /* g_base_info_unref (base_info); */

  return 0;

}

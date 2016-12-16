#ifndef __CAMERAPATHLOADER
#define __CAMERAPATHLOADER

#include "objload/obj_parser.h"
#include "camera.h"

int obj_parse_cameras_file(std::vector<Camera*>* cameras, std::vector<int>* times, obj_growable_scene_data *growable_data, const char *filename)
{
	FILE* obj_file_stream;
	int current_material = -1;
	char *current_token = NULL;
	char current_line[OBJ_LINE_SIZE];
	int line_number = 0;
	// open scene
	obj_file_stream = fopen( filename, "r");
	if(obj_file_stream == 0)
	{
		fprintf(stderr, "Error reading file: %s\n", filename);
		return 0;
	}
	
	char const * lastSlash = filename;
	while( NULL != strstr(lastSlash, "/"))
		lastSlash = strstr(lastSlash, "/")+1;
	size_t slashPos = lastSlash - filename;
	
	/*
	 extreme_dimensions[0].x = INFINITY; extreme_dimensions[0].y = INFINITY; extreme_dimensions[0].z = INFINITY;
	 extreme_dimensions[1].x = -INFINITY; extreme_dimensions[1].y = -INFINITY; extreme_dimensions[1].z = -INFINITY;
	 
	 if(v->x < extreme_dimensions[0].x) extreme_dimensions[0].x = v->x;
	 if(v->x > extreme_dimensions[1].x) extreme_dimensions[1].x = v->x;
	 if(v->y < extreme_dimensions[0].y) extreme_dimensions[0].y = v->y;
	 if(v->y > extreme_dimensions[1].y) extreme_dimensions[1].y = v->y;
	 if(v->z < extreme_dimensions[0].z) extreme_dimensions[0].z = v->z;
	 if(v->z > extreme_dimensions[1].z) extreme_dimensions[1].z = v->z;*/
	
	
	//parser loop
	while( fgets(current_line, OBJ_LINE_SIZE, obj_file_stream) )
	{
		current_token = strtok( current_line, " \t\n\r");
		line_number++;
		
		//skip comments
		if( current_token == NULL || current_token[0] == '#')
			continue;
		
		//parse objects
		else if( strequal(current_token, "v") ) //process vertex
		{
			list_add_item(&growable_data->vertex_list,  obj_parse_vector(), NULL);
		}
		
		else if( strequal(current_token, "vn") ) //process vertex normal
		{
			list_add_item(&growable_data->vertex_normal_list,  obj_parse_vector(), NULL);
		}
		
		// else if( strequal(current_token, "vt") ) //process vertex texture
		// {
		// 	list_add_item(&growable_data->vertex_texture_list,  obj_parse_vector(), NULL);
		// }
		
		// else if( strequal(current_token, "f") ) //process face
		// {
		// 	obj_face *face = obj_parse_face(growable_data);
		// 	face->material_index = current_material;
		// 	list_add_item(&growable_data->face_list, face, NULL);
		// }
		
		// else if( strequal(current_token, "sp") ) //process sphere
		// {
		// 	obj_sphere *sphr = obj_parse_sphere(growable_data);
		// 	sphr->material_index = current_material;
		// 	list_add_item(&growable_data->sphere_list, sphr, NULL);
		// }
		
		// else if( strequal(current_token, "pl") ) //process plane
		// {
		// 	obj_plane *pl = obj_parse_plane(growable_data);
		// 	pl->material_index = current_material;
		// 	list_add_item(&growable_data->plane_list, pl, NULL);
		// }
		
		// else if( strequal(current_token, "p") ) //process point
		// {
		// 	//make a small sphere to represent the point?
		// }
		
		// else if( strequal(current_token, "lp") ) //light point source
		// {
		// 	obj_light_point *o = obj_parse_light_point(growable_data);
		// 	o->material_index = current_material;
		// 	list_add_item(&growable_data->light_point_list, o, NULL);
		// }
		
		// else if( strequal(current_token, "ld") ) //process light disc
		// {
		// 	obj_light_disc *o = obj_parse_light_disc(growable_data);
		// 	o->material_index = current_material;
		// 	list_add_item(&growable_data->light_disc_list, o, NULL);
		// }
		
		// else if( strequal(current_token, "lq") ) //process light quad
		// {
		// 	obj_light_quad *o = obj_parse_light_quad(growable_data);
		// 	o->material_index = current_material;
		// 	list_add_item(&growable_data->light_quad_list, o, NULL);
		// }
		
		else if( strequal(current_token, "c") ) //camera
		{
			growable_data->camera = (obj_camera*) malloc(sizeof(obj_camera));
			obj_parse_camera(growable_data, growable_data->camera);

			Camera* toAdd = new Camera(growable_data->camera, growable_data->vertex_list, growable_data->vertex_normal_list);
			cameras->push_back(toAdd);
		}

		else if( strequal(current_token, "t") ) //time
		{
			char* toParse = strtok(NULL, " \n\t");
			int toCheck = itoa(toparse);
			if(int > 0) {
				times->push_back(toCheck);
			}

		}
		
		// else if( strequal(current_token, "usemtl") ) // usemtl
		// {
		// 	current_material = list_find(&growable_data->material_list, strtok(NULL, WHITESPACE));
		// }
		
		// else if( strequal(current_token, "mtllib") ) // mtllib
		// {
		// 	strncpy(growable_data->material_filename, filename, slashPos);
		// 	strncpy(growable_data->material_filename+slashPos, strtok(NULL, WHITESPACE), OBJ_FILENAME_LENGTH-slashPos);
		// 	obj_parse_mtl_file(growable_data->material_filename, &growable_data->material_list);
		// 	continue;
		// }
		
		else if( strequal(current_token, "o") ) //object name
		{ }
		else if( strequal(current_token, "s") ) //smoothing
		{ }
		else if( strequal(current_token, "g") ) // group
		{ }
		
		else
		{
			printf("Unknown command '%s' in scene code at line %i: \"%s\".\n",
				   current_token, line_number, current_line);
		}
	}
	
	fclose(obj_file_stream);
	
	return 1;
}

int load(const char *filename, std::vector<obj_camera*>* cameras, std::vector<int>* times)
	{
		obj_scene_data data;
		int no_error = 1;
		no_error = parse_obj_camera_file(std::vector<obj_camera*>* cameras, std::vector<int>* times, &data, filename);
		if(no_error)
		{

		}
		
		return no_error;
	}

#endif;
// Interval Tree.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "interval_tree.h"

int main()
{
	interval_tree course_table;
	char cmd = 'A';
	while (cmd != 'Q') {
		cout << "==================== Course Management System ====================\n";
		cout << "Insert a course :		I\n";
		cout << "Delete a course :		D\n";
		cout << "Search a course :		S\n";
		cout << "Print current courses :		P\n";
		cout << "Quit :				Q\n";
		cout << "==================================================================\n";
		cout << "Please input command:\n";


		cin >> cmd;

		switch (cmd)
		{
			case 'I': {
				cout << "Please input course ID\n";
				int ID;
				cin >> ID;
				cout << "Please input course name\n";
				string name;
				getchar();
				cin >> name;
				cout << "Please input course time interval\n";
				int time_start, time_end;
				cin >> time_start >> time_end;
				//TO DO
				course_table.insert_data(time_start, time_end, ID, name);
				cout << "Finish!\n\n";
			}
			break;
				
			case 'D':{
				cout << "Please input delete interval\n";
				int time_start, time_end;
				cin >> time_start >> time_end;
				//TO DO
				course_table.delete_data(time_start, time_end);
				cout << "Finish!\n\n";
			}
			break;
			case 'S': {
				cout << "\n";
				cout << "Please input search interval\n";
				int time_start, time_end;
				cin >> time_start >> time_end;
				//TO DO
				course_table.search_data(time_start, time_end);
				cout << "Finish!\n\n";
			}
			break;
			case 'P': {
				//TO DO
				cout << "Please choose print method: I(In-order) or L(Layer)\n";
				char option;
				cin >> option;
				cout << "Current courses :\n";
				if (option == 'I')
					course_table.print_inorder();
				else if (option == 'L')
					course_table.print_layer();
				else
					cout << "\nInvalid option!\n\n";
			}
			break;
			case 'Q': {
				cout << "Thank you, bye!\n";
			}
			break;
			default: {
				cout << "\nInvalid command!\n\n";
			}
			break;
		}
	}
    return 0;
}


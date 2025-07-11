#include "Stack.h"
#include <string>
struct Operation
{
	Operation() {};
	Operation(bool type, int position, string text)
	{
		this->position = position;
		this->text = text;
		this->type = type;
	}
	bool type; // TRUE FOR INSERT, FALSE FOR DELETE
	int position;
	string text;
};

void Insert(int position, string text, string& main_text)
{
	main_text.insert(position, text);
}

string Delete(int position, int range, string& main_text)
{
	string temp_text = main_text.substr(position, range);
	main_text.erase(position, range);
	return temp_text;
}

void Undo(Stack<Operation>& undo, Stack<Operation>& redo, string& main_text)
{
	redo.push(undo.peek());
	if (undo.peek().type)
	{
		Delete(undo.peek().position, undo.peek().text.size(), main_text);
	}
	else if (!undo.peek().type)
	{
		Insert(undo.peek().position, undo.peek().text, main_text);
	}
	undo.pop();
}

void Redo(Stack<Operation>& undo, Stack<Operation>& redo, string& main_text)
{
	undo.push(redo.peek());
	if (redo.peek().type)
	{
		Insert(redo.peek().position, redo.peek().text, main_text);
	}
	else if (!redo.peek().type)
	{
		Delete(redo.peek().position, redo.peek().text.size(), main_text);
	}
	redo.pop();
}

int main()
{
	string main_text = "";
	Stack<Operation> undo, redo;
	while (true)
	{
		cout << "Please write your operation (INSERT / DELETE / UNDO / REDO / SHOW / EXIT): " << endl;
		string choice;
		cin >> choice;
		if (choice == "INSERT")
		{
			cout << "Enter text`s position: ";
			int position;
			if (!(cin >> position)) {
				cout << "Invalid input." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			if (position > main_text.size() || position < 0)
			{
				cout << "Invalid position." << endl;
			}
			else
			{
				cout << "Enter your text: " << endl;
				string text;
				getline(cin >> ws, text);
				Insert(position, text, main_text);
				undo.push(Operation(true, position, text));
				while (!redo.isEmpty())
				{
					redo.pop();
				}
			}
		}
		else if (choice == "DELETE")
		{
			cout << "Enter start of deletion area: ";
			int position;
			if (!(cin >> position)) {
				cout << "Invalid numeric input." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			if (position > main_text.size() || position < 0)
			{
				cout << "Invalid position." << endl;
			}
			else
			{
				cout << "Enter range of deletion: ";
				int range;
				if (!(cin >> range)) {
					cout << "Invalid input." << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					continue;
				}
				if ((position + range) > main_text.size() || range < 1)
				{
					cout << "Invalid range." << endl;
				}
				else
				{
					undo.push(Operation(false, position, Delete(position, range, main_text)));
					while (!redo.isEmpty())
					{
						redo.pop();
					}
				}
			}
		}
		else if (choice == "UNDO")
		{
			if (undo.isEmpty())
			{
				cout << "No tasks to undo." << endl;
			}
			else
			{
				Undo(undo, redo, main_text);
			}
		}
		else if (choice == "REDO")
		{
			if (redo.isEmpty())
			{
				cout << "No tasks to redo." << endl;
			}
			else
			{
				Redo(undo, redo, main_text);
			}
		}
		else if (choice == "SHOW")
		{
			cout << main_text << endl;
		}
		else if (choice == "EXIT")
		{
			break;
		}
		else
		{
			cout << "Invalid command." << endl;
		}
	}
	return 0;
}
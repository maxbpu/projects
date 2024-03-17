from flask import Flask, render_template, request, redirect, url_for, jsonify, abort
from flask_sqlalchemy import SQLAlchemy
import sys
from flask_migrate import Migrate

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://yaopu@localhost:5432/todoapp'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

migrate = Migrate(app, db)

class Todo(db.Model): 
    __tablename__ = 'todos'
    id = db.Column(db.Integer, primary_key=True)
    description = db.Column(db.String(), nullable=False)
    completed = db.Column(db.Boolean, nullable=False, default=False)
    list_id = db.Column(db.Integer, db.ForeignKey('todolists.id', ondelete='CASCADE'), nullable=False) # syntax is <tablename of parent>.<column name of child primary key>

    def __repr__(self):
        return f'<Todo {self.id} {self.description}>'

# Todo list
class TodoList(db.Model):
    __tablename__ = 'todolists'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(), nullable=False)
    todos = db.relationship('Todo', backref='list', lazy=True, cascade='all, delete-orphan') # name of the child class; custom name for referencing the parent


# CREATE a list
@app.route('/lists/create', methods=['POST'])
def create_list():
    error = False
    body = {}
    try:
        name = request.get_json()['name'] 
        todolist = TodoList(name=name)
        db.session.add(todolist)
        db.session.commit()
        body['id'] = todolist.id
        body['name'] = todolist.name
    except:
        error = True
        db.session.rollback()
    finally:
        db.session.close()
    if error:
        abort (400) 
    else:
        return jsonify(body) 


# CREATE an item
@app.route('/todos/create', methods=['POST'])
def create_todo():
    error = False
    body = {}
    print("CREATE an item")
    try:
        description = request.get_json()['description'] 
        list_id = request.get_json()['list-id']
        todo = Todo(description=description, list_id=list_id)
        db.session.add(todo)
        db.session.commit()
        body['id'] = todo.id
        body['description'] = todo.description
        body['completed'] = todo.completed
    except:
        print("EXCEPT")
        error = True
        db.session.rollback()
        print(sys.exc_info())
    finally:
        db.session.close()
    if error:
        abort (400) 
    else:
        return jsonify(body) 
    



# COMPLETE a list
@app.route('/lists/<list_id>/set-completed', methods=['POST'])
def set_completed_list(list_id): 
    print("set_completed_list function")
    print("list_id is ", list_id)
    error = False
    try:
        todos = Todo.query.filter_by(list_id=list_id)
        print(todos)
        for todo in todos:
            print(todo)
            todo.completed = True
        db.session.commit()
        print("session commit")
    except:
        print("session rollback")
        db.session.rollback()
        error = True
    finally:
        print("session close")
        db.session.close()
    if error:
        abort(500)
    else:
        return '', 200



# COMPLETE an item
@app.route('/todos/<todo_id>/set-completed', methods=['POST']) 
def set_completed_todo(todo_id): 
    try:
        completed = request.get_json()['completed']
        todo = db.session.get(Todo, todo_id)
        todo.completed = completed 
        db.session.commit()
    except:
        db.session.rollback()
    finally:
        db.session.close()
    return redirect(url_for('index')) # refresh (which actually doesn't work with fetch)



# DELETE a list
@app.route('/lists/<list_id>/delete', methods=['DELETE'])
def delete_list(list_id):
    try:
        print("list delete")
        todolist = db.session.get(TodoList, list_id)
        db.session.delete(todolist)
        print("list delete commit")
        db.session.commit()
    except:
        print("list delete rollback")
        db.session.rollback()
    finally:
        print("list delete close")
        db.session.close()
    print("list delete return")
    return jsonify({ 'success': True })


# DELETE an item
@app.route('/todos/<todo_id>/delete', methods=['DELETE'])
def delete_todo(todo_id):
    try:
        Todo.query.filter_by(id=todo_id).delete()
        db.session.commit()
    except:
        db.session.rollback()
    finally:
        db.session.close()
    return jsonify({ 'success': True }) # cant redirect to homepage, because the DELETE method doesn't allow it






"""
Previously, at the homepage, the user sees the entirety of the todo itmes in the database
Now: instead of going to the homepage, we use '/lists/<list_id>' for each list, and returns all the todos in list with list_id
"""
@app.route('/lists/<list_id>')
def get_list_todos(list_id):
    return render_template('index.html', 
                           lists=TodoList.query.all(), # left column data
                           active_list=db.session.get(TodoList, list_id),
                           todos=Todo.query.filter_by(list_id=list_id).order_by('id').all()) # right column data

@app.route('/')
def index():
    return redirect(url_for('get_list_todos', list_id=TodoList.query.first().id))



if __name__ == '__main__':
   app.debug = True
   app.run(host="0.0.0.0")
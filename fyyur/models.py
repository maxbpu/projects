#----------------------------------------------------------------------------#
# Imports
#----------------------------------------------------------------------------#

from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

#----------------------------------------------------------------------------#
# Models.
#----------------------------------------------------------------------------#

class Venue(db.Model):
    __tablename__ = 'Venue'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String)
    city = db.Column(db.String(120))
    state = db.Column(db.String(120))
    address = db.Column(db.String(120))
    phone = db.Column(db.String(120))
    image_link = db.Column(db.String(500))
    genres = db.Column(db.String()) #
    facebook_link = db.Column(db.String(120))
    website_link = db.Column(db.String()) #
    seeking_talent = db.Column(db.Boolean) #
    seeking_description = db.Column(db.String()) #

    show = db.relationship('Show', backref='venue')

    # TODO: implement any missing fields, as a database migration using Flask-Migrate

class Artist(db.Model):
    __tablename__ = 'Artist'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String)
    city = db.Column(db.String(120))
    state = db.Column(db.String(120))
    phone = db.Column(db.String(120))
    image_link = db.Column(db.String(500))
    genres = db.Column(db.String(120))
    facebook_link = db.Column(db.String(120))
    website_link = db.Column(db.String()) #
    seeking_venue = db.Column(db.Boolean) #
    seeking_description = db.Column(db.String()) #

    show = db.relationship('Show', backref='artist')

    # TODO: implement any missing fields, as a database migration using Flask-Migrate

# TODO Implement Show and Artist models, and complete all model relationships and properties, as a database migration.

class Show(db.Model): #
    __tablename__ = 'Show'

    id = db.Column(db.Integer, primary_key=True) #
    artist_id = db.Column(db.Integer, db.ForeignKey('Artist.id')) #
    venue_id = db.Column(db.Integer, db.ForeignKey('Venue.id')) #
    start_time = db.Column(db.DateTime) #
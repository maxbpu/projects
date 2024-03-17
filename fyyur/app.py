#----------------------------------------------------------------------------#
# Imports
#----------------------------------------------------------------------------#

import json
import dateutil.parser
import babel
from flask import Flask, render_template, request, Response, flash, redirect, url_for
from flask_moment import Moment
from flask_sqlalchemy import SQLAlchemy
import logging
from logging import Formatter, FileHandler
from flask_wtf import Form
from forms import *

from flask_migrate import Migrate
from models import *
from datetime import datetime

#----------------------------------------------------------------------------#
# App Config.
#----------------------------------------------------------------------------#

from models import db

app = Flask(__name__)
moment = Moment(app)
app.config.from_object('config')
db.init_app(app)

migrate = Migrate(app, db)

#----------------------------------------------------------------------------#
# Filters.
#----------------------------------------------------------------------------#

def format_datetime(value, format='medium'):
  if isinstance(value, str):
      date = dateutil.parser.parse(value)
  else:
      date = value
  if format == 'full':
      format="EEEE MMMM, d, y 'at' h:mma"
  elif format == 'medium':
      format="EE MM, dd, y h:mma"
  return babel.dates.format_datetime(date, format, locale='en')

app.jinja_env.filters['datetime'] = format_datetime

#----------------------------------------------------------------------------#
# Controllers.
#----------------------------------------------------------------------------#

@app.route('/')
def index():
  return render_template('pages/home.html')


#  Venues
#  ----------------------------------------------------------------


@app.route('/venues')
def venues():
  data = []
  city_state_list = db.session.query(Venue.city, Venue.state).distinct(Venue.city, Venue.state).all()
  for city, state in city_state_list:
    data_ele = {}
    data_ele["city"] = city
    data_ele["state"] = state
    venue_list = []
    venues = db.session.query(Venue).filter_by(city=city, state=state).with_entities(Venue.id, Venue.name).all()
    for id, name in venues:
      venue_dict = {}
      venue_dict["id"] = id
      venue_dict["name"] = name
      venue_dict["num_upcoming_shows"] = db.session.query(Show).filter_by(venue_id=id).count()
      venue_list.append(venue_dict)
    data_ele["venues"] = venue_list
    data.append(data_ele)
  return render_template('pages/venues.html', areas=data)


@app.route('/venues/search', methods=['POST'])
def search_venues():
  search_term = request.form.get('search_term', '')
  response = {}
  data = []
  venue_list = Venue.query.with_entities(Venue.id, Venue.name).all()
  count = 0
  for venue_id, venue_name in venue_list:
    if search_term.casefold() in venue_name.casefold():
      count = count+1
      data_ele = {}
      data_ele["id"] = venue_id
      data_ele["name"] = venue_name
      data_ele["num_upcoming_shows"] = Show.query.filter_by(venue_id=venue_id).count()
      data.append(data_ele)
  response["count"] = count
  response["data"] = data
  return render_template('pages/search_venues.html', results=response, search_term=search_term)
  

@app.route('/venues/<int:venue_id>')
def show_venue(venue_id):
  venueId = venue_id
  data = {}
  data["past_shows"] = []
  data["upcoming_shows"] = []
  data["past_shows_count"] = 0
  data["upcoming_shows_count"] = 0
  venue = Venue.query.with_entities(Venue.id, Venue.name, Venue.genres, Venue.address, Venue.city, Venue.state, Venue.phone, Venue.website_link, Venue.facebook_link, Venue.seeking_talent, Venue.seeking_description, Venue.image_link).filter_by(id=venueId).all()
  for id, name, genres, address, city, state, phone, website_link, facebook_link, seeking_talent, seeking_description, image_link in venue:
    data["id"] = id
    data["name"] = name
    data["genres"] = [genres]
    data["address"] = address
    data["city"] = city
    data["state"] = state
    data["phone"] = phone
    data["website"] = website_link
    data["facebook_link"] = facebook_link
    data["seeking_talent"] = seeking_talent
    data["seeking_description"] = seeking_description
    data["image_link"] = image_link
  show = Show.query.filter_by(venue_id=venueId).with_entities(Show.artist_id, Show.start_time).all()
  now = datetime.now()
  for artist_id, start_time in show:
    artist = {}
    artist["artist_id"] = artist_id
    artist["artist_name"] = Artist.query.get(artist_id).name
    artist["artist_image_link"] = Artist.query.get(artist_id).image_link
    artist["start_time"] = start_time
    if (now<start_time):
      data["upcoming_shows"].append(artist)
      data["upcoming_shows_count"] = data["upcoming_shows_count"]+1
    else:
      data["past_shows"].append(artist)
      data["past_shows_count"] = data["past_shows_count"]+1
  return render_template('pages/show_venue.html', venue=data)


#  Create Venue
#  ----------------------------------------------------------------


@app.route('/venues/create', methods=['GET'])
def create_venue_form():
  form = VenueForm()
  return render_template('forms/new_venue.html', form=form)


@app.route('/venues/create', methods=['POST'])
def create_venue_submission():
  form = VenueForm(request.form)
  try:
    if form.validate():
      name = form.name.data
      city = form.city.data
      state = form.state.data
      address = form.address.data
      phone = form.phone.data
      image_link = form.image_link.data
      genres = ','.join(str(x) for x in form.genres.data)
      facebook_link = form.facebook_link.data
      website_link = form.website_link.data
      seeking_talent = form.seeking_talent.data
      seeking_description = form.seeking_description.data
      if db.session.query(Venue).filter_by(name=name, city=city, state=state, address=address, phone=phone, image_link=image_link, genres=genres, facebook_link=facebook_link, website_link=website_link, seeking_talent=seeking_talent, seeking_description=seeking_description).count()<1:
        venue = Venue(name=name, city=city, state=state, address=address, phone=phone, image_link=image_link, genres=genres, facebook_link=facebook_link, website_link=website_link, seeking_talent=seeking_talent, seeking_description=seeking_description)
        db.session.add(venue)
        db.session.commit()
        flash('Venue ' + request.form['name'] + ' was successfully listed!')
  except:
    db.session.rollback()
    flash('An error occurred. Venue ' + form.name.data + ' could not be listed.')
  finally:
    db.session.close()
  return render_template('pages/home.html')


#  Artists
#  ----------------------------------------------------------------


@app.route('/artists')
def artists():
  data_record = Artist.query.with_entities(Artist.id, Artist.name).all()
  data = []
  for id, name in data_record:
    data_dict = {}
    data_dict["id"] = id
    data_dict["name"] = name
    data.append(data_dict)
  return render_template('pages/artists.html', artists=data)


@app.route('/artists/search', methods=['POST'])
def search_artists():
  search_term = request.form.get('search_term', '')
  response = {}
  data = []
  artist_list = Artist.query.with_entities(Artist.id, Artist.name).all()
  count = 0
  for artist_id, artist_name in artist_list:
    if search_term.casefold() in artist_name.casefold():
      count = count+1
      data_ele = {}
      data_ele["id"] = artist_id
      data_ele["name"] = artist_name
      data_ele["num_upcoming_shows"] = Show.query.filter_by(artist_id=artist_id).count()
      data.append(data_ele)
  response["count"] = count
  response["data"] = data
  return render_template('pages/search_artists.html', results=response, search_term=search_term)
  

@app.route('/artists/<int:artist_id>')
def show_artist(artist_id):
  artistId = artist_id
  data = {}
  data["past_shows"] = []
  data["upcoming_shows"] = []
  data["past_shows_count"] = 0
  data["upcoming_shows_count"] = 0
  artist = Artist.query.with_entities(Artist.id, Artist.name, Artist.genres, Artist.city, Artist.state, Artist.phone, Artist.website_link, Artist.facebook_link, Artist.seeking_venue, Artist.seeking_description, Artist.image_link).filter_by(id=artistId).all()
  for id, name, genres, city, state, phone, website_link, facebook_link, seeking_venue, seeking_description, image_link in artist:
    data["id"] = id
    data["name"] = name
    data["genres"] = [genres]
    data["city"] = city
    data["state"] = state
    data["phone"] = phone
    data["website"] = website_link
    data["facebook_link"] = facebook_link
    data["seeking_venue"] = seeking_venue
    data["seeking_description"] = seeking_description
    data["image_link"] = image_link
  show = Show.query.filter_by(artist_id=artistId).with_entities(Show.venue_id, Show.start_time).all()
  now = datetime.now()
  for venue_id, start_time in show:
    venue = {}
    venue["venue_id"] = venue_id
    venue["venue_name"] = Venue.query.get(venue_id).name
    venue["venue_image_link"] = Venue.query.get(venue_id).image_link
    venue["start_time"] = start_time
    if (now<start_time):
      data["upcoming_shows"].append(venue)
      data["upcoming_shows_count"] = data["upcoming_shows_count"]+1
    else:
      data["past_shows"].append(venue)
      data["past_shows_count"] = data["past_shows_count"]+1
  return render_template('pages/show_artist.html', artist=data)


#  Create Artist
#  ----------------------------------------------------------------


@app.route('/artists/create', methods=['GET'])
def create_artist_form():
  form = ArtistForm()
  return render_template('forms/new_artist.html', form=form)

@app.route('/artists/create', methods=['POST'])
def create_artist_submission():
  form = ArtistForm(request.form)
  try:
    if form.validate():
      name = form.name.data
      city = form.city.data
      state = form.state.data
      phone = form.phone.data
      image_link = form.image_link.data
      genres = ','.join(str(x) for x in form.genres.data)
      facebook_link = form.facebook_link.data
      website_link = form.website_link.data
      seeking_venue = form.seeking_venue.data
      seeking_description = form.seeking_description.data
      if db.session.query(Artist).filter_by(name=name, city=city, state=state, phone=phone, image_link=image_link, genres=genres, facebook_link=facebook_link, website_link=website_link, seeking_venue=seeking_venue, seeking_description=seeking_description).count()<1:
        artist = Artist(name=name, city=city, state=state, phone=phone, image_link=image_link, genres=genres, facebook_link=facebook_link, website_link=website_link, seeking_venue=seeking_venue, seeking_description=seeking_description)
        db.session.add(artist)
        db.session.commit()
        flash('Artist ' + request.form['name'] + ' was successfully listed!')
  except:
    db.session.rollback()
    flash('An error occurred. Artist ' + form.name.data + ' could not be listed.')
  finally:
    db.session.close()
  return render_template('pages/home.html')


#  Shows
#  ----------------------------------------------------------------


@app.route('/shows')
def shows():
  showList = Show.query.join(Venue, Show.venue_id==Venue.id).join(Artist, Show.artist_id==Artist.id)
  showData = showList.with_entities(Venue.id, Venue.name, Artist.id, Artist.name, Artist.image_link, Show.start_time).all()
  data = []
  for venue_id, venue_name, artist_id, artis_name, artist_image_link, show_start_time in showData:
    data_dict = {}
    data_dict["venue_id"] = venue_id
    data_dict["venue_name"] = venue_name
    data_dict["artist_id"] = artist_id
    data_dict["artist_name"] = artis_name
    data_dict["artist_image_link"] = artist_image_link
    data_dict["start_time"] = show_start_time
    data.append(data_dict)
  return render_template('pages/shows.html', shows=data)

@app.route('/shows/create')
def create_shows():
  # renders form. do not touch.
  form = ShowForm()
  return render_template('forms/new_show.html', form=form)

@app.route('/shows/create', methods=['POST'])
def create_show_submission():
  form = ShowForm(request.form)
  try:
    if form.validate():
      artist_id = form.artist_id.data
      venue_id = form.venue_id.data
      start_time = form.start_time.data
      if db.session.query(Show).filter_by(artist_id=artist_id, venue_id=venue_id, start_time=start_time).count()<1:
        show = Show(artist_id=artist_id, venue_id=venue_id, start_time=start_time)
        db.session.add(show)
        db.session.commit()
        flash('Show was successfully listed!')
  except:
    db.session.rollback()
    flash('An error occurred. Show could not be listed.')
  finally:
    db.session.close()
  return render_template('pages/home.html')

@app.errorhandler(404)
def not_found_error(error):
    return render_template('errors/404.html'), 404

@app.errorhandler(500)
def server_error(error):
    return render_template('errors/500.html'), 500


if not app.debug:
    file_handler = FileHandler('error.log')
    file_handler.setFormatter(
        Formatter('%(asctime)s %(levelname)s: %(message)s [in %(pathname)s:%(lineno)d]')
    )
    app.logger.setLevel(logging.INFO)
    file_handler.setLevel(logging.INFO)
    app.logger.addHandler(file_handler)
    app.logger.info('errors')

#----------------------------------------------------------------------------#
# Launch.
#----------------------------------------------------------------------------#

# Default port:
if __name__ == '__main__':
    app.run()


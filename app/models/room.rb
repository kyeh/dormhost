class Room < ActiveRecord::Base
  belongs_to :host
  has_many :room_reviews
  belongs_to :college
  has_one :room_profile
  has_many :favorites
  
  validates_presence_of :street_address_1, :city, :state, :zip, :message => "must be filled in"
  validates_numericality_of :zip
  validates_length_of :zip, :is => 5, :message => "must be exactly 5 digits"
  
end

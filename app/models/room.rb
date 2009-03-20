class Room < ActiveRecord::Base
  acts_as_xapian :texts => [:street_address_1, :city, :state]

  belongs_to :host
  has_many :room_reviews
  belongs_to :college
  has_one :room_profile
  has_many :favorites
  
  attr_accessor :q
  
  validates_presence_of :street_address_1, :city, :state, :zip, :college_id, :message => "must be filled in"
  validates_numericality_of :zip
  validates_length_of :zip, :is => 5, :message => "must be exactly 5 digits" 
  
end

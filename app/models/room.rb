class Room < ActiveRecord::Base
  acts_as_xapian  :texts => [:street_address_1, :city, :state, :zip],
                  :terms => [[:city, 'C', "city"],
                            [:state, 'S', "state"],
                            [:zip, 'Q', "zip"],
                            [:college_id, 'O', "college"]]

  belongs_to :host
  has_many :room_reviews
  belongs_to :college
  has_one :room_profile
  has_many :favorites
  has_many :transactions
  
  attr_accessor :q, :filter
  
  validates_presence_of :street_address_1, :city, :state, :zip, :college_id, :message => "must be filled in"
  validates_numericality_of :zip
  validates_length_of :zip, :is => 5, :message => "must be exactly 5 digits" 
  
end

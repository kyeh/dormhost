class Room < ActiveRecord::Base
  
  acts_as_rateable
  acts_as_xapian  :texts =>   [:street_address_1, :city, :state, :zip],
                  :terms =>   [[:city, 'C', "city"],
                              [:state, 'S', "state"],
                              [:zip, 'Q', "zip"],
                              [:host, 'H', "host"],
                              [:college_id, 'O', "college"]]

  belongs_to :host
  belongs_to :college
  has_one :room_profile
  has_many :favorites
  has_many :transactions
  
  attr_accessor :q, :filter
  
  validates_presence_of :street_address_1, :city, :state, :zip, :college_id, :message => "must be filled in"
  validates_numericality_of :zip
  validates_length_of :zip, :is => 5, :message => "must be exactly 5 digits" 


    cattr_reader :per_page
    @@per_page = 5
    
  def full_address
    "#{street_address_1}, #{city}, #{state}, #{zip}"
  end
end
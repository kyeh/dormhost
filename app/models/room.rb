class Room < ActiveRecord::Base
  belongs_to :host
  has_many :room_reviews
  belongs_to :college
  has_one :room_profile
  has_many :favorites
  
end

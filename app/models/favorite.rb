class Favorite < ActiveRecord::Base
  belongs_to :room
  belongs_to :renter
end
